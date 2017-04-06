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
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace BugFSK
{
    public class Project
    {
        //
        int nextBugID;

        //
        string projectID;

        //
        string projectName;

        //
        List<Bug> projectBugs;

        /// <summary>
        /// 
        /// </summary>
        /// <param name="nextBugID"></param>

        public Project(string projectName, string projectID, int autoIncrementValue, Bug[] bugs)
        {
            this.projectName = projectName;
            this.projectID = projectID;
            this.nextBugID = autoIncrementValue;

            // can't pass a null argument to the List's constructor - results in an exception.
            if (bugs != null)
            {
                projectBugs = new List<Bug>(bugs);
            }
            else
            {
                projectBugs = new List<Bug>();
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="newBug"></param>
        public void AddBug(Bug newBug)
        {
            if (projectBugs.Count(x => x.ID == newBug.ID) > 0)
            {
                throw new MatchingIDException();
            }
            else
            {
                projectBugs.Add(newBug);

                nextBugID++;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bugID"></param>
        public void RemoveBug(string bugID)
        {
            projectBugs.RemoveAll(x => x.ID == bugID);
        }

        /// <summary>
        /// 
        /// </summary>
        public string ID
        {
            get { return projectID; }
            set { projectID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        public string Name
        {
            get { return projectName; }
            set { projectName = value; }
        }

        ///// <summary>
        ///// 
        ///// </summary>
        public Bug[] GetBugs()
        {
            if (projectBugs != null)
            {
                return projectBugs.ToArray();
            }

            return null;
        }

        public int NextIDValue
        {
            get { return nextBugID; }
            //set { nextBugID = value; }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bug"></param>
        public void UpdateIssue(Bug bug)
        {
            for (int i = 0; i < projectBugs.Count; i++)
            {
                if (bug.ID == projectBugs[i].ID)
                {
                    projectBugs[i] = bug;
                }
            }
        }

        public Bug GetIssue(string issueID)
        {
            Bug b = projectBugs.Where(x => x.ID == issueID).First();

            return b;
        }

        internal void UpdateID(string newID)
        {
            string oldID = this.projectID;
            this.projectID = newID;

            for (int i = 0; i < projectBugs.Count; i++)
            {
                projectBugs[i].ID = projectBugs[i].ID.Replace(oldID, newID);
            }
        }
    }
}
