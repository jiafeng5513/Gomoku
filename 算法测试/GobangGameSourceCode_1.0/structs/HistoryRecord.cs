using System;
using System.Collections.Generic;
using System.Text;

namespace GoBangProject.structs
{
    public class HistoryRecord
    {

        #region 成员变量
        
        private ChessPoint point;

        private String text;

        #endregion

        public HistoryRecord(ChessPoint point)
        {
            this.text = point.Type+"\t\tX:" + point.Point.X + "\tY:" + point.Point.Y;
            this.point = point;
        }

        #region 属性
        public String Text
        {
            get { return text; }
        }

        public ChessPoint Point
        {
            get { return point; }
        }
        #endregion
    }
}
