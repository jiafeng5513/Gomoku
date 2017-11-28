using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Reflection;
using System.IO;

namespace FiveStone
{
    public class Stones
    {
        private Graphics mg;
        private Bitmap bs;
        private Bitmap ws;

        public Stones(Graphics g)
        {
            Assembly myAssembly = Assembly.GetExecutingAssembly();
            Stream bStream = myAssembly.GetManifestResourceStream("FiveStone.black.png");
            Stream wStream = myAssembly.GetManifestResourceStream("FiveStone.white.png");
            bs = new Bitmap(bStream);
            ws = new Bitmap(wStream);
            bStream.Close();
            wStream.Close();
            mg = g;
        }

        public void DrawStone(int x, int y, bool flag)
        {
            if (flag)
            {
                mg.DrawImage(bs, x * 40 + 20, y * 40 + 23, bs.Width, bs.Height);
            }
            else
            {
                mg.DrawImage(ws, x * 40 + 20, y * 40 + 23, bs.Width, bs.Height);
            }
        }
    }
}
