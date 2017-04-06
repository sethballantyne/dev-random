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
    public partial class NewIssue : Form
    {
        //bool editMode = false;
        IssueDialogMode dialogMode = IssueDialogMode.Create;
        Bug issue = null;

        Point buttonLocation = new Point();
        public NewIssue()
        {
            InitializeComponent();

            PopulateProjectComboBox();
        }

        public NewIssue(IssueDialogMode dialogMode, Bug selectedIssue)
            : this()
        {
            this.dialogMode = dialogMode;

            if (selectedIssue == null)
            {
                throw new ArgumentNullException();
            }
            else
            {
                this.issue = selectedIssue;

                if (dialogMode == IssueDialogMode.View)
                {
                    buttonLocation = okButton.Location;
                    okButton.Location = cancelButton.Location;
                    cancelButton.Visible = false;

                    FillInFields(selectedIssue);
                    // we're entering "view" mode which has its own set of controls.
                    priorityComboBox.Enabled = false;
                    projectComboBox.Enabled = false;
                    issueTypeComboBox.Enabled = false;

                    titleTextBox.ReadOnly = true;
                    descriptionRichTextBox.ReadOnly = true;
                    descriptionRichTextBox.BackColor = System.Drawing.SystemColors.Window;
                    titleTextBox.BackColor = System.Drawing.SystemColors.Window;

                    editButton.Visible = true;
                }
                else if(dialogMode == IssueDialogMode.Edit)
                {
                    FillInFields(selectedIssue);
                }
            }
        }

        private void FillInFields(Bug selectedIssue)
        {
            //if (dialogMode == IssueDialogMode.Edit)
            //{
                titleTextBox.Text = selectedIssue.Title;
                descriptionRichTextBox.Text = selectedIssue.Description;

                priorityComboBox.SelectedItem = selectedIssue.Priority.ToString();
                issueTypeComboBox.SelectedItem = selectedIssue.Type.ToString();

                string projectID = selectedIssue.ID.Split(new char[] { '-' }).First();
                string projectName = Omnius.GetProject(projectID).Name;
                projectComboBox.SelectedItem = projectName;
            //}
        }

        private void PopulateProjectComboBox()
        {
            Project[] projects = Omnius.GetProjects();
            for (int i = 0; i < projects.Length; i++)
            {
                projectComboBox.Items.Add(projects[i].Name);
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            if (dialogMode == IssueDialogMode.View)
            {
                // No need to update the database, so pretend we clicked "cancel"
                // as in edit/create mode.
                this.DialogResult = DialogResult.Cancel;
            }
            else
            {

                string projectName = projectComboBox.SelectedItem as string;
                Project p = Omnius.GetProjects().Where(x => x.Name == projectName).First();

                Bug newIssue = new Bug();
                newIssue.Description = descriptionRichTextBox.Text;

                // The ID shouldn't change if we're editing an existing issue.
                if (dialogMode == IssueDialogMode.Create)
                {
                    newIssue.ID = p.ID + "-" + p.NextIDValue;
                }
                else
                {
                    newIssue.ID = issue.ID;
                }

                newIssue.Priority = (Priority)Enum.Parse(typeof(Priority), priorityComboBox.SelectedItem as string);
                newIssue.Title = titleTextBox.Text;
                newIssue.Type = issueTypeComboBox.SelectedItem as string;

                if (dialogMode == IssueDialogMode.Create) // creating a new issue
                {
                    Omnius.Add(newIssue, p.ID);
                }
                else if (dialogMode == IssueDialogMode.Edit)// editing an existing issue
                {
                    Omnius.Update(newIssue, p.ID);
                }

                DialogResult = DialogResult.OK;
            }
        }

        private void titleTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateOKButtonStatus();
        }

        private void UpdateOKButtonStatus()
        {
            if (titleTextBox.Text.Length > 0 &&
                issueTypeComboBox.SelectedIndex > -1 &&
                projectComboBox.SelectedIndex > -1 &&
                priorityComboBox.SelectedIndex > -1 &&
                descriptionRichTextBox.Text.Length > 0)
            {
                okButton.Enabled = true;
            }
            else
            {
                okButton.Enabled = false;
            }
        }

        private void descriptionRichTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateOKButtonStatus();
        }

        private void projectComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateOKButtonStatus();
        }

        private void priorityComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateOKButtonStatus();
        }

        private void issueTypeComboBox_SelectedIndexChanged(object sender, EventArgs e)
        {
            UpdateOKButtonStatus();
        }

        private void editButton_Click(object sender, EventArgs e)
        {
            dialogMode = IssueDialogMode.Edit;

            priorityComboBox.Enabled = true;
            projectComboBox.Enabled = true;
            issueTypeComboBox.Enabled = true;

            titleTextBox.ReadOnly = false;
            descriptionRichTextBox.ReadOnly = false;

            editButton.Visible = false;

            cancelButton.Visible = true;
            okButton.Location = buttonLocation;
        }

        private void descriptionContextMenuStrip_Opening(object sender, CancelEventArgs e)
        {
            if (descriptionRichTextBox.SelectedText.Length == 0)
            {
                cutToolStripMenuItem.Enabled = false;
                deleteToolStripMenuItem.Enabled = false;
                copyToolStripMenuItem.Enabled = false;
            }
            else
            {
                cutToolStripMenuItem.Enabled = true;
                copyToolStripMenuItem.Enabled = true;

                // Don't bother enabling the menu item since we can't delete text
                // if the control is read only.
                if (descriptionRichTextBox.ReadOnly == false)
                {
                    deleteToolStripMenuItem.Enabled = true;
                }
                else
                {
                    deleteToolStripMenuItem.Enabled = false;
                }
            }
            //else if(descriptionRichTextBox.

            if (descriptionRichTextBox.ReadOnly)
            {
                pasteToolStripMenuItem.Enabled = false;
            }
            else
            {
                pasteToolStripMenuItem.Enabled = true;
            }

            if (!descriptionRichTextBox.Focused)
                descriptionRichTextBox.Focus();
        }

        private void selectAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (descriptionRichTextBox.Text.Length > 0)
            {
                descriptionRichTextBox.SelectAll();
                copyToolStripMenuItem.Enabled = true;

                if (!descriptionRichTextBox.ReadOnly)
                {
                    cutToolStripMenuItem.Enabled = true;
                }
            }
        }

        private void copyToolStripMenuItem_Click(object sender, EventArgs e)
        {
            descriptionRichTextBox.Copy();
        }

        private void cutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            descriptionRichTextBox.Cut();
        }

        private void pasteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            descriptionRichTextBox.Paste();
            descriptionRichTextBox.DeselectAll();
        }

        private void deleteToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (descriptionRichTextBox.SelectedRtf.Length > 0)
            {
                descriptionRichTextBox.SelectedRtf = "";
            }

            if (descriptionRichTextBox.SelectedText.Length > 0)
            {
                descriptionRichTextBox.SelectedText = "";
            }
        }
    }
}
