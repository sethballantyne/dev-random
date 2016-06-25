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
using System.Globalization;

namespace BugFSK
{
    public partial class NewProject : Form
    {
        Project projectBeingEdited = null;
        //Project newProject = null;
        bool performIDUpdate = false;
        bool performNameUpdate = false;

        Mode dialogMode = Mode.Create;

        public NewProject()
        {
            InitializeComponent();
        }

        public NewProject(Mode dialogMode, Project project) : this()
        {
            this.dialogMode = dialogMode;
            this.projectBeingEdited = project;

            if (dialogMode == Mode.Edit)
            {
                if (project != null)
                {
                    projectNameTextBox.Text = project.Name;
                    idTextBox.Text = project.ID;
                }
            }
        }

        private void okButton_Click(object sender, EventArgs e)
        {
            string errorMessage = null;
            string errorCaption = "Unable to create project.";
            Project[] projects = Omnius.GetProjects();

            if (dialogMode == Mode.Create)
            {
                foreach (Project p in projects)
                {
                    if (p.Name.ToLower() == projectNameTextBox.Text.ToLower())
                    {
                        errorMessage = "A project with the name " + projectNameTextBox.Text + " already exists.";
                        MessageBox.Show(errorMessage, errorCaption, MessageBoxButtons.OK, MessageBoxIcon.Error);

                        projectNameTextBox.Focus();
                        return;
                    }
                    else if (p.ID.ToLower() == idTextBox.Text.ToLower())
                    {
                        errorMessage = "A project with the ID " + idTextBox.Text + " already exists.";
                        MessageBox.Show(errorMessage, errorCaption, MessageBoxButtons.OK, MessageBoxIcon.Error);

                        idTextBox.Focus();
                        return;
                    }
                    else
                    {
                        Omnius.Add(new Project(projectNameTextBox.Text, idTextBox.Text, 1, null));

                        DialogResult = DialogResult.OK;
                    }
                }
            }
            else // Mode.Edit
            {
                // if idTextBox does not match projectBeingEdited.ID then the ID has been
                // changed. Search for new ID in Omnius. If ID in Omnius exists, display error
                // and reset idTextBox to projectBeingEdited.ID. 
                if(idTextBox.Text.ToLower(CultureInfo.CurrentCulture) !=
                   projectBeingEdited.ID.ToLower(CultureInfo.CurrentCulture))
                {
                    if (projects != null)
                    {
                        foreach (Project project in projects)
                        {
                            if (project.ID.ToLower(CultureInfo.CurrentCulture) ==
                                idTextBox.Text.ToLower(CultureInfo.CurrentCulture))
                            {
                                errorMessage = "A project with the ID " + idTextBox.Text + " already exists.";
                                MessageBox.Show(errorMessage, errorCaption, MessageBoxButtons.OK, MessageBoxIcon.Error);

                                idTextBox.Text = projectBeingEdited.ID;

                                return;
                            }
                        }

                        // ID does not exist in Omnius; safe to update.
                        performIDUpdate = true;
                    }
                }
                // do same for project name.
                if (projectNameTextBox.Text.ToLower(CultureInfo.CurrentCulture) !=
                   projectBeingEdited.Name.ToLower(CultureInfo.CurrentCulture))
                {
                    // Project name has been changed. Names are unique, so we need
                    // to make sure it doesn't already exist in Omnius.
                    if (projects != null)
                    {
                        foreach (Project project in projects)
                        {
                            if (project.Name.ToLower(CultureInfo.CurrentCulture) ==
                                projectNameTextBox.Text.ToLower(CultureInfo.CurrentCulture))
                            {
                                // A project with the selected name already exists, reset the name back
                                // to its original.
                                errorMessage = "A project with the name " + projectNameTextBox.Text + " already exists.";
                                MessageBox.Show(errorMessage, errorCaption, MessageBoxButtons.OK, MessageBoxIcon.Error);

                                projectNameTextBox.Text = projectBeingEdited.Name;

                                return;
                            }
                            
                        }

                        // haven't found a matching name, so OK to update Omnius.
                        performNameUpdate = true;
                    }

                }

                if((!performNameUpdate) &&
                   (!performIDUpdate))
                {
                    // nothing to update
                    DialogResult = DialogResult.Cancel;
                }

                // If ID in Omnius has to be updated, update ID.
                // if project Name in omnius has to be updated, updated Name.
                string newName = null;
                string newID = null;

                if (performIDUpdate)
                {
                    newID = idTextBox.Text; 
                }

                if (performNameUpdate)
                {
                    newName = projectNameTextBox.Text;
                }

                Omnius.Update(projectBeingEdited.ID, newID, newName);

                DialogResult = DialogResult.OK;
            }
        }

        private void idTextBox_TextChanged(object sender, EventArgs e)
        {
            if ((idTextBox.Text.Length > 0) &&
               (projectNameTextBox.Text.Length > 0))
            {
                okButton.Enabled = true;
            }
            else
            {
                okButton.Enabled = false;
            }
        }
    }
}
