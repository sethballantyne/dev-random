/*Copyright (c) 2014 Seth Ballantyne <seth.ballantyne@gmail.com>
*
*Permission is hereby granted, free of charge, to any person obtaining a copy
*of this software and associated documentation files (the "Software"), to deal
*in the Software without restriction, including without limitation the rights
*to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*copies of the Software, and to permit persons to whom the Software is
*furnished to do so, subject to the following conditions:
*
*The above copyright notice and this permission notice shall be included in
*all copies or substantial portions of the Software.
*
*THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
*THE SOFTWARE.
*/

using System;
using System.Collections;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BugFSK
{
    class ListViewPriorityColumnComparer : IComparer
    {
        private int col;
        SortOrder sortOrder = SortOrder.Descending;

        public ListViewPriorityColumnComparer()
        {
            col = 0;
        }

        public ListViewPriorityColumnComparer(int column)
        {
            col = column;
        }

        public ListViewPriorityColumnComparer(int column, SortOrder sortOrder) : this(column)
        {
            this.sortOrder = sortOrder;
        }

        public int Compare(object x, object y)
        {
            try
            {
                string xText = ((ListViewItem)x).SubItems[col].Text;
                string yText = ((ListViewItem)y).SubItems[col].Text;

                Priority a = (Priority)Enum.Parse(typeof(Priority), xText);
                Priority b = (Priority)Enum.Parse(typeof(Priority), yText);
                //int a = Convert.ToInt32(((ListViewItem)x).SubItems[col].Text);
                //int b = Convert.ToInt32(((ListViewItem)y).SubItems[col].Text);

                if (sortOrder == SortOrder.Descending)
                {
                    return b.CompareTo(a);
                }
                else
                {
                    return a.CompareTo(b);
                }
            }
            catch (OverflowException)
            {
                return 0;
            }
            catch (FormatException)
            {
                string a = ((ListViewItem)x).SubItems[col].Text;
                string b = ((ListViewItem)y).SubItems[col].Text;

                return String.Compare(a, b);
            }
        }
    }
}
