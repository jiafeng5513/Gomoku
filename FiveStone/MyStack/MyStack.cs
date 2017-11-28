using System;
using System.Collections.Generic;
using System.Text;

namespace FiveStone.MyStack
{
    public class MyStack
    {
        public const int MAXSIZE = 5000;
        public struct FSStack
        {
            public string[] data;
            public int top;
        }

        public FSStack st = new FSStack();

        public void CreatStack()
        {
            st.data = new string[MAXSIZE];
            st.top = -1;
        }

        public bool Insert(string str)
        {
            if (st.top == MAXSIZE - 1)
            {
                return false;
            }
            else
            {
                st.top++;
                st.data[st.top] = str;
                return true;
            }
        }

        public bool Delete()
        {
            if (st.top == -1)
            {
                return false;
            }
            else
            {
                st.top--;
                return true;
            }
        }

        public string GetTop()
        {
            return st.data[st.top].ToString();
        }

        public string GetPC()
        {
            if (st.top != -1)
            {
                return st.data[st.top].ToString();
            }
            else
            {
                return "";
            }
        }

        public string GetPerson()
        {
            if (st.top != -1&&st.top>0)
            {
                return st.data[st.top-1].ToString();
            }
            else
            {
                return "";
            }
        }
    }
}
