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
    public partial class ManageProjects : Form
    {
        bool changesMade = false;

        private void UpdateListView()
        {
            projectsListView.Items.Clear();

            Project[] projects = Omnius.GetProjects();
            if (projects != null)
            {
                List<string> itemList = new List<string>();
                for (int i = 0; i < projects.Length; i++)
                {
                    itemList.Add(projects[i].ID);
                    itemList.Add(projects[i].Name);

                    ListViewItem listViewItem = new ListViewItem(itemList.ToArray());
                    projectsListView.Items.Add(listViewItem);

                    itemList.Clear();
                }
            }

            deleteButton.Enabled = false;
            renameButton.Enabled = false;
        }

        public ManageProjects()
        {
            InitializeComponent();

            UpdateListView();
        }

        private void addButton_Click(object sender, EventArgs e)
        {
            if (new NewProject().ShowDialog() == DialogResult.OK)
            {
                XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                UpdateListView();

                changesMade = true;
            }
        }

        private void projectsListView_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (projectsListView.SelectedItems.Count > 0)
            {
                renameButton.Enabled = true;
                deleteButton.Enabled = true;
            }
            else
            {
                renameButton.Enabled = false;
                deleteButton.Enabled = false;
            }
        }

        private void renameButton_Click(object sender, EventArgs e)
        {
            string projectID = projectsListView.SelectedItems[0].Text;
            Project selectedProject = Omnius.GetProjects().Where(
                x => x.ID == projectID).First();

            if (selectedProject != null)
            {
                NewProject newProject = new NewProject(Mode.Edit, selectedProject);
                if (newProject.ShowDialog() == DialogResult.OK)
                {
                    XMLFile.Write("XMLFile.xml", Omnius.GetProjects());

                    UpdateListView();

                    changesMade = true;
                }
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (changesMade)
            {
                DialogResult = DialogResult.OK;
            }
            else
            {
                DialogResult = DialogResult.Cancel;
            }
        }

        private void deleteButton_Click(object sender, EventArgs e)
        {
            string projectName = projectsListView.SelectedItems[0].SubItems[1].Text;
            string message = "This action is irreversible. Are you sure you want to delete the project " + projectName + "?";

            DialogResult dr = MessageBox.Show(message, "Continue?", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);
            if (dr == DialogResult.Yes)
            {
                string projectID = projectsListView.SelectedItems[0].Text;
                Omnius.DeleteProject(projectID);

                UpdateListView();

                

                XMLFile.Write("XMLFile.xml", Omnius.GetProjects());
                changesMade = true;
            }
        }
        
    }
}
