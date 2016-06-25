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
    ///////////////////////////////////////////////////////////////////////////////////////////////
    // 
    // The spice extends life. The spice expands conciousness. The spice is vital to space travel.
    //
    ///////////////////////////////////////////////////////////////////////////////////////////////
    static class Omnius
    {
        static List<Project> projects = new List<Project>();

        /// <summary>
        /// 
        /// </summary>
        /// <param name="project"></param>
        public static void Add(Project project)
        {
            try
            {
                if (projects.Count(x => x.ID == project.ID) == 0)
                {
                    projects.Add(project);
                }
            }
            catch
            {
                throw;
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="projects"></param>
        public static void Add(Project[] projects)
        {
            try
            {
                foreach (Project p in projects)
                {
                    Add(p);
                }
            }
            catch
            {
                throw;
            }
        }

        public static void Add(Bug[] bugs, string projectID)
        {
            foreach (Bug b in bugs)
            {
                Add(b, projectID);
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bug"></param>
        /// <param name="projectID"></param>
        public static void Add(Bug bug, string projectID)
        {
            projects.Where(x => x.ID == projectID).ToArray()[0].AddBug(bug);
        }

        public static Project[] GetProjects()
        {
            return projects.ToArray();
        }

        public static void Clear()
        {
            projects.Clear();
        }

        /// <summary>
        /// Marks the specified issue as closed.
        /// </summary>
        /// <param name="issueID">ID of the issue to close.</param>
        /// <param name="projectID">ID of the project the issue belongs to.</param>
        public static void Close(string issueID, string projectID)
        {
            Project project = projects.Where(x => x.ID == projectID).First();
            if (project != null)
            {
                project.GetBugs().Where(x => x.ID == issueID).First().Status = Status.Closed;

                XMLFile.Write("XMLFile.xml", projects.ToArray());
            }
            else
            {
                // error - no project with the specified id.
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="bug"></param>
        /// <param name="projectID"></param>
        public static void Update(Bug bug, string projectID)
        {
            for (int i = 0; i < projects.Count; i++)
            {
                if (projects[i].ID == projectID)
                {
                    projects[i].UpdateIssue(bug);

                    XMLFile.Write("XMLFile.xml", projects.ToArray());

                    return;
                }
            }
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="oldProjectID"></param>
        /// <param name="newProjectID"></param>
        /// <param name="newProjectName"></param>
        public static void Update(string oldProjectID, string newProjectID, string newProjectName)
        {
            if (oldProjectID == null)
            {
                throw new ArgumentNullException("oldProjectID can't be null.");
            }
            else if (newProjectID == null && newProjectName == null)
            {
                // nothing to do
                return;
            }

            for (int i = 0; i < projects.Count; i++)
            {
                if (projects[i].ID == oldProjectID)
                {
                    if (newProjectID != null)
                    {
                        projects[i].UpdateID(newProjectID);
                    }

                    if (newProjectName != null)
                    {
                        projects[i].Name = newProjectName;
                    }

                    return;
                }
            }
        }

        public static Bug GetIssue(string issueID)
        {
            string projectID = issueID.Split(new char[] { '-' }).First();
            Project project = projects.Where(x => x.ID == projectID).First();

            return project.GetIssue(issueID);
        }

        public static Project GetProject(string projectID)
        {
            Project p = projects.Where(x => x.ID == projectID).First();

            return p;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="projectID"></param>
        internal static void DeleteProject(string projectID)
        {
            if (projectID != null)
            {
                if (projects != null)
                {
                    for (int i = 0; i < projects.Count; i++)
                    {
                        if (projects[i].ID == projectID)
                        {
                            projects.RemoveAt(i);

                            return;
                        }
                    }
                }
            }
            else
            {
                throw new ArgumentNullException("projectID is null.");
            }
        }
    }
}
