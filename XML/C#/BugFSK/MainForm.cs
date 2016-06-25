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
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace BugFSK
{
    public partial class MainForm : Form
    {
        // sort order for the "priority" column
        SortOrder issuesListViewSortOrder = SortOrder.Descending;

        public MainForm()
        {
            InitializeComponent();

            Project[] projects = XMLFile.Read("XMLFile.xml");
            if (projects.Length > 0)
            {
                Omnius.Add(projects);
            }

            PopulateProjectsComboBox();
        }

        private void PopulateProjectsComboBox()
        {
            projectsComboBox.Items.Clear();
            projectsComboBox.Items.Add("All Projects");

            Project[] projects = Omnius.GetProjects();
            foreach (Project p in projects)
            {
                projectsComboBox.Items.Add(p.Name);
            }

            projectsComboBox.SelectedIndex = 0;
        }

        private void PopulateIssuesListView(Bug[] bugs)
        {
            issuesListView.Items.Clear();

            // iterating with foreach will result in a crash if bugs is null.
            if (bugs != null)
            {
                string[] items = new string[issuesListView.Columns.Count];
                foreach (Bug b in bugs)
                {
                    if (b.Status == Status.Open)
                    {
                        items[0] = b.ID;
                        items[1] = b.Type;
                        items[2] = b.Title;
                        items[3] = b.Priority.ToString();
                        items[4] = b.Status.ToString();


                        issuesListView.Items.Add(new ListViewItem(items));
                    }
                }
            }
        }

        private void bugToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (new NewIssue().ShowDialog() == DialogResult.OK)
            {
                XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                UpdateIssuesListView(); 
            }
        }

        private void UpdateIssuesListView()
        {
            if (projectsComboBox.SelectedIndex == 0)
            {
                Project[] projects = Omnius.GetProjects();
                if (projects != null)
                {
                    List<Bug> bugs = new List<Bug>();
                    foreach (Project p in Omnius.GetProjects())
                    {
                        bugs.AddRange(p.GetBugs());
                    }

                    PopulateIssuesListView(bugs.ToArray());
                }
            }
            else
            {
                string selectedProject = projectsComboBox.SelectedItem as string;
                IEnumerable<Project> temp = Omnius.GetProjects().Where(x => x.Name == selectedProject);

                if (temp.Count() > 0)
                {
                    PopulateIssuesListView(temp.First().GetBugs());
                }
            }
        }

        private void projectToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (new NewProject().ShowDialog() == DialogResult.OK)
            {
                // Update the XML file with the new project and refresh the combo
                // box so the project is selectable.
                XMLFile.Write("XMLFile.xml", Omnius.GetProjects());
                PopulateProjectsComboBox();
            }
        }

        private void projectsComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            
            if (projectsComboBox.SelectedIndex == 0)
            {
                // "All Projects" selected - so list all open issues for all projects.
                Project[] projects = Omnius.GetProjects();
                List<Bug> bugs = new List<Bug>();
                for (int i = 0; i < projects.Length; i++)
                {
                    Bug[] projectBugs = projects[i].GetBugs();
                    if (projectBugs != null && projectBugs.Length > 0)
                    {
                        //if (projectBugs.Length > 0)
                        //{
                            bugs.AddRange(projects[i].GetBugs());
                        //}
                    }
                }

                PopulateIssuesListView(bugs.ToArray());
            }
            else
            {
                string selectedText = projectsComboBox.SelectedItem as string;
                Project[] projects = Omnius.GetProjects();
                for (int i = 0; i < projects.Length; i++)
                {
                    if (projects[i].Name == selectedText)
                    {
                        PopulateIssuesListView(projects[i].GetBugs());
                    }
                }
            }
        }

        private void closeContextMenuStripItem_Click(object sender, EventArgs e)
        {
            DialogResult dr = MessageBox.Show("Close issue?", "Confirmation needed.", 
                MessageBoxButtons.YesNo, MessageBoxIcon.Question);

            if (dr == DialogResult.Yes)
            {
                // grab the ID of the selected issue
                string selectedIssue = issuesListView.SelectedItems[0].Text;
                string projectID = selectedIssue.Split(new char[] { '-' }).First();

                //find the issue within Omnius, mark it as closed.
                Omnius.Close(selectedIssue, projectID);

                issuesListView.Items.Remove(issuesListView.SelectedItems[0]);
            }
        }

        private void listViewContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            if (issuesListView.SelectedItems.Count > 0)
            {
                closeContextMenuStripItem.Enabled = true;
                viewContextMenuStripItem.Enabled = true;
                editContextMenuStripItem.Enabled = true;
            }
            else
            {
                closeContextMenuStripItem.Enabled = false;
                viewContextMenuStripItem.Enabled = false;
                editContextMenuStripItem.Enabled = false;
            }
        }

        private void editContextMenuStripItem_Click(object sender, EventArgs e)
        {
            string selectedID = issuesListView.SelectedItems[0].Text;
            Bug selectedIssue = Omnius.GetIssue(selectedID);

            if (selectedIssue != null)
            {
                if (new NewIssue(IssueDialogMode.Edit, selectedIssue).ShowDialog() == DialogResult.OK)
                {
                    XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                    UpdateIssuesListView();
                }
            }
        }

        private void issuesListView_ColumnClick(object sender, ColumnClickEventArgs e)
        {
            if (issuesListView.Columns[e.Column].Text != "Priority")
            {
                issuesListView.ListViewItemSorter = new ListViewItemComparer(e.Column);
            }
            else
            {
                issuesListView.ListViewItemSorter = new ListViewPriorityColumnComparer(e.Column, issuesListViewSortOrder);
                if (issuesListViewSortOrder == SortOrder.Descending)
                {
                    issuesListViewSortOrder = SortOrder.Ascending;
                }
                else
                {
                    issuesListViewSortOrder = SortOrder.Descending;
                }
            }
        }

        private void viewContextMenuStripItem_Click(object sender, EventArgs e)
        {
            //new IssueDlg(IssueDialogMode.View).Show();
            string selectedID = issuesListView.SelectedItems[0].Text;
            Bug selectedIssue = Omnius.GetIssue(selectedID);

            if (selectedIssue != null)
            {
                if (new NewIssue(IssueDialogMode.View, selectedIssue).ShowDialog() == DialogResult.OK)
                {
                    XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                    UpdateIssuesListView();
                }
            }
        }

        private void issuesListView_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            if (issuesListView.SelectedItems.Count > 0)
            {
                string selectedID = issuesListView.SelectedItems[0].Text;
                Bug selectedIssue = Omnius.GetIssue(selectedID);

                if (selectedIssue != null)
                {
                    // ShowDialog will never return DialogResult.OK while in view mode, but it's possible
                    // to change the mode within the form and return a DialogResult.OK value when in Edit/Create
                    // mode, so we need to check for it.
                    if (new NewIssue(IssueDialogMode.View, selectedIssue).ShowDialog() == DialogResult.OK)
                    {
                        XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                        UpdateIssuesListView();
                    }
                }
            }
        }

        private void manageProjectsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ManageProjects manageProjects = new ManageProjects();
            if (manageProjects.ShowDialog() == DialogResult.OK)
            {
                UpdateIssuesListView();
                PopulateProjectsComboBox();
            }
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void newIssueToolStripButton_Click(object sender, EventArgs e)
        {
            if (new NewIssue().ShowDialog() == DialogResult.OK)
            {
                XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                UpdateIssuesListView();
            }
        }

        
    }
}
