# 캡처 폴더 두 개를 픽셀 단위로 비교한다(intent/render-capture).
# 사용: powershell.exe -NoProfile -File scripts/compare-shots.ps1 -Expected <기준 폴더> -Actual <비교 폴더> [-Tolerance <채널 차이>] [-MaxRatio <비율>]
#  - 픽셀의 R·G·B 중 하나라도 차이가 Tolerance 를 넘으면 「다른 픽셀」이다. 알파는 화면에 보이지 않으므로 비교하지 않는다.
#  - 이미지의 다른 픽셀 비율이 MaxRatio 를 넘거나, 크기가 다르거나, 한쪽에만 있으면 실패다.
#  - 다른 픽셀이 있는 이미지는 <Actual>/diff/ 에 표시 이미지를 만든다(같은 픽셀은 어둡게, 다른 픽셀은 빨강).
#  - 종료 코드: 모두 통과 0, 하나라도 실패 1.
# 출력은 ASCII 로 쓴다 — WSL 에서 부르면 한글 출력이 깨진다.
param(
	[Parameter(Mandatory = $true)][string]$Expected,
	[Parameter(Mandatory = $true)][string]$Actual,
	[int]$Tolerance = 0,
	[double]$MaxRatio = 0
)
$ErrorActionPreference = 'Stop'

Add-Type -AssemblyName System.Drawing
# 픽셀 비교는 PowerShell 반복문으로는 느리다 — C# 로 한 번에 비교한다
Add-Type -ReferencedAssemblies System.Drawing -TypeDefinition @'
using System;
using System.Drawing;
using System.Drawing.Imaging;
using System.Runtime.InteropServices;

public static class ShotDiff
{
	// 반환: { 다른 픽셀 수, 최대 채널 차이, 전체 픽셀 수 }. 크기가 다르면 다른 픽셀 수 -1.
	public static long[] Compare(string expectedPath, string actualPath, int tolerance, string diffPath)
	{
		using (Bitmap expected = new Bitmap(expectedPath))
		using (Bitmap actual = new Bitmap(actualPath))
		{
			int w = expected.Width;
			int h = expected.Height;
			if (w != actual.Width || h != actual.Height)
			{
				return new long[] { -1, -1, (long)w * h };
			}
			Rectangle rect = new Rectangle(0, 0, w, h);
			byte[] pe = ReadPixels(expected, rect);
			byte[] pa = ReadPixels(actual, rect);
			int stride = pe.Length / h;

			byte[] marked = new byte[pe.Length];
			long diffCount = 0;
			long maxDiff = 0;
			for (int y = 0; y < h; ++y)
			{
				for (int x = 0; x < w; ++x)
				{
					int i = y * stride + x * 4;
					int channelDiff = 0;
					for (int c = 0; c < 3; ++c)	// B, G, R — 알파(3)는 비교하지 않는다
					{
						channelDiff = Math.Max(channelDiff, Math.Abs(pe[i + c] - pa[i + c]));
					}
					maxDiff = Math.Max(maxDiff, channelDiff);
					if (channelDiff > tolerance)
					{
						++diffCount;
						marked[i + 2] = 255;	// 빨강
					}
					else
					{
						marked[i] = (byte)(pa[i] / 4);
						marked[i + 1] = (byte)(pa[i + 1] / 4);
						marked[i + 2] = (byte)(pa[i + 2] / 4);
					}
					marked[i + 3] = 255;
				}
			}
			if (diffCount > 0 && diffPath != null)
			{
				using (Bitmap diff = new Bitmap(w, h, PixelFormat.Format32bppArgb))
				{
					BitmapData data = diff.LockBits(rect, ImageLockMode.WriteOnly, PixelFormat.Format32bppArgb);
					Marshal.Copy(marked, 0, data.Scan0, marked.Length);
					diff.UnlockBits(data);
					diff.Save(diffPath, ImageFormat.Png);
				}
			}
			return new long[] { diffCount, maxDiff, (long)w * h };
		}
	}

	static byte[] ReadPixels(Bitmap bitmap, Rectangle rect)
	{
		BitmapData data = bitmap.LockBits(rect, ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
		byte[] pixels = new byte[data.Stride * rect.Height];
		Marshal.Copy(data.Scan0, pixels, 0, pixels.Length);
		bitmap.UnlockBits(data);
		return pixels;
	}
}
'@

# .NET 은 PowerShell 의 현재 위치를 모르므로 절대 경로로 바꿔 넘긴다
$expectedDir = (Resolve-Path -LiteralPath $Expected).ProviderPath
$actualDir = (Resolve-Path -LiteralPath $Actual).ProviderPath
$diffDir = Join-Path $actualDir 'diff'
if (Test-Path -LiteralPath $diffDir) { Remove-Item -LiteralPath $diffDir -Recurse -Force }
New-Item -ItemType Directory -Path $diffDir | Out-Null

$names = @(Get-ChildItem -LiteralPath $expectedDir -Filter *.png | ForEach-Object Name) +
	@(Get-ChildItem -LiteralPath $actualDir -Filter *.png | ForEach-Object Name) | Sort-Object -Unique
if ($names.Count -eq 0)
{
	Write-Output 'FAIL no png files'
	exit 1
}

$failCount = 0
foreach ($name in $names)
{
	$expectedPath = Join-Path $expectedDir $name
	$actualPath = Join-Path $actualDir $name
	if (-not (Test-Path -LiteralPath $actualPath)) { Write-Output "FAIL $name missing in actual"; ++$failCount; continue }
	if (-not (Test-Path -LiteralPath $expectedPath)) { Write-Output "FAIL $name missing in expected"; ++$failCount; continue }

	$result = [ShotDiff]::Compare($expectedPath, $actualPath, $Tolerance, (Join-Path $diffDir $name))
	if ($result[0] -lt 0)
	{
		Write-Output "FAIL $name size differs"
		++$failCount
		continue
	}
	$ratio = $result[0] / $result[2]
	$verdict = if ($ratio -le $MaxRatio) { 'PASS' } else { 'FAIL' }
	if ($verdict -eq 'FAIL') { ++$failCount }
	Write-Output ('{0} {1} diff={2} ratio={3:0.000000} maxChannelDiff={4}' -f $verdict, $name, $result[0], $ratio, $result[1])
}

if (@(Get-ChildItem -LiteralPath $diffDir).Count -eq 0) { Remove-Item -LiteralPath $diffDir }
Write-Output ('RESULT {0} (fail={1}, tolerance={2}, maxRatio={3})' -f $(if ($failCount -eq 0) { 'PASS' } else { 'FAIL' }), $failCount, $Tolerance, $MaxRatio)
exit $(if ($failCount -eq 0) { 0 } else { 1 })
