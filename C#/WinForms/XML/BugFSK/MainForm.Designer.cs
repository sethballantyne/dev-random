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

namespace BugFSK
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.fileToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.newToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.issueToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.projectToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.editToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.manageProjectsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem4 = new System.Windows.Forms.ToolStripMenuItem();
            this.helpToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.aboutToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel1 = new System.Windows.Forms.Panel();
            this.issuesListView = new System.Windows.Forms.ListView();
            this.idColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.typeColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.titleColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.priorityColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.statusColumnHeader = new System.Windows.Forms.ColumnHeader();
            this.listViewContextMenuStrip = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.viewContextMenuStripItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripSeparator();
            this.editContextMenuStripItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.closeContextMenuStripItem = new System.Windows.Forms.ToolStripMenuItem();
            this.openContextMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.panel2 = new System.Windows.Forms.Panel();
            this.label1 = new System.Windows.Forms.Label();
            this.projectsComboBox = new System.Windows.Forms.ComboBox();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.newIssueToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.newProjectToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.manageProjectsToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.menuStrip1.SuspendLayout();
            this.panel1.SuspendLayout();
            this.listViewContextMenuStrip.SuspendLayout();
            this.panel2.SuspendLayout();
            this.toolStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.fileToolStripMenuItem,
            this.editToolStripMenuItem,
            this.toolStripMenuItem4,
            this.helpToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(736, 24);
            this.menuStrip1.TabIndex = 2;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // fileToolStripMenuItem
            // 
            this.fileToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newToolStripMenuItem,
            this.toolStripMenuItem1,
            this.exitToolStripMenuItem});
            this.fileToolStripMenuItem.Name = "fileToolStripMenuItem";
            this.fileToolStripMenuItem.Size = new System.Drawing.Size(37, 20);
            this.fileToolStripMenuItem.Text = "&File";
            // 
            // newToolStripMenuItem
            // 
            this.newToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.issueToolStripMenuItem,
            this.projectToolStripMenuItem});
            this.newToolStripMenuItem.Name = "newToolStripMenuItem";
            this.newToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.newToolStripMenuItem.Text = "&New";
            // 
            // issueToolStripMenuItem
            // 
            this.issueToolStripMenuItem.Name = "issueToolStripMenuItem";
            this.issueToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.issueToolStripMenuItem.Text = "&Issue";
            this.issueToolStripMenuItem.Click += new System.EventHandler(this.bugToolStripMenuItem_Click);
            // 
            // projectToolStripMenuItem
            // 
            this.projectToolStripMenuItem.Name = "projectToolStripMenuItem";
            this.projectToolStripMenuItem.Size = new System.Drawing.Size(111, 22);
            this.projectToolStripMenuItem.Text = "&Project";
            this.projectToolStripMenuItem.Click += new System.EventHandler(this.projectToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(95, 6);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(98, 22);
            this.exitToolStripMenuItem.Text = "E&xit";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // editToolStripMenuItem
            // 
            this.editToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.manageProjectsToolStripMenuItem});
            this.editToolStripMenuItem.Name = "editToolStripMenuItem";
            this.editToolStripMenuItem.Size = new System.Drawing.Size(39, 20);
            this.editToolStripMenuItem.Text = "&Edit";
            // 
            // manageProjectsToolStripMenuItem
            // 
            this.manageProjectsToolStripMenuItem.Name = "manageProjectsToolStripMenuItem";
            this.manageProjectsToolStripMenuItem.Size = new System.Drawing.Size(162, 22);
            this.manageProjectsToolStripMenuItem.Text = "Manage Projects";
            this.manageProjectsToolStripMenuItem.Click += new System.EventHandler(this.manageProjectsToolStripMenuItem_Click);
            // 
            // toolStripMenuItem4
            // 
            this.toolStripMenuItem4.Name = "toolStripMenuItem4";
            this.toolStripMenuItem4.Size = new System.Drawing.Size(44, 20);
            this.toolStripMenuItem4.Text = "View";
            // 
            // helpToolStripMenuItem
            // 
            this.helpToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.aboutToolStripMenuItem});
            this.helpToolStripMenuItem.Name = "helpToolStripMenuItem";
            this.helpToolStripMenuItem.Size = new System.Drawing.Size(44, 20);
            this.helpToolStripMenuItem.Text = "Help";
            // 
            // aboutToolStripMenuItem
            // 
            this.aboutToolStripMenuItem.Name = "aboutToolStripMenuItem";
            this.aboutToolStripMenuItem.Size = new System.Drawing.Size(150, 22);
            this.aboutToolStripMenuItem.Text = "About BugFSK";
            // 
            // panel1
            // 
            this.panel1.Controls.Add(this.issuesListView);
            this.panel1.Controls.Add(this.panel2);
            this.panel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panel1.Location = new System.Drawing.Point(0, 49);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(736, 331);
            this.panel1.TabIndex = 3;
            // 
            // issuesListView
            // 
            this.issuesListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.idColumnHeader,
            this.typeColumnHeader,
            this.titleColumnHeader,
            this.priorityColumnHeader,
            this.statusColumnHeader});
            this.issuesListView.ContextMenuStrip = this.listViewContextMenuStrip;
            this.issuesListView.Dock = System.Windows.Forms.DockStyle.Fill;
            this.issuesListView.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.issuesListView.FullRowSelect = true;
            this.issuesListView.Location = new System.Drawing.Point(0, 27);
            this.issuesListView.Name = "issuesListView";
            this.issuesListView.Size = new System.Drawing.Size(736, 304);
            this.issuesListView.TabIndex = 1;
            this.issuesListView.UseCompatibleStateImageBehavior = false;
            this.issuesListView.View = System.Windows.Forms.View.Details;
            this.issuesListView.MouseDoubleClick += new System.Windows.Forms.MouseEventHandler(this.issuesListView_MouseDoubleClick);
            this.issuesListView.ColumnClick += new System.Windows.Forms.ColumnClickEventHandler(this.issuesListView_ColumnClick);
            // 
            // idColumnHeader
            // 
            this.idColumnHeader.Text = "ID";
            this.idColumnHeader.Width = 93;
            // 
            // typeColumnHeader
            // 
            this.typeColumnHeader.Text = "Type";
            this.typeColumnHeader.Width = 121;
            // 
            // titleColumnHeader
            // 
            this.titleColumnHeader.Text = "Title";
            this.titleColumnHeader.Width = 650;
            // 
            // priorityColumnHeader
            // 
            this.priorityColumnHeader.Text = "Priority";
            this.priorityColumnHeader.Width = 133;
            // 
            // statusColumnHeader
            // 
            this.statusColumnHeader.Text = "Status";
            // 
            // listViewContextMenuStrip
            // 
            this.listViewContextMenuStrip.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.viewContextMenuStripItem,
            this.toolStripMenuItem2,
            this.editContextMenuStripItem,
            this.toolStripMenuItem3,
            this.closeContextMenuStripItem,
            this.openContextMenuItem});
            this.listViewContextMenuStrip.Name = "listViewContextMenuStrip";
            this.listViewContextMenuStrip.Size = new System.Drawing.Size(104, 104);
            this.listViewContextMenuStrip.Opening += new System.ComponentModel.CancelEventHandler(this.listViewContextMenuStrip_Opening);
            // 
            // viewContextMenuStripItem
            // 
            this.viewContextMenuStripItem.Name = "viewContextMenuStripItem";
            this.viewContextMenuStripItem.Size = new System.Drawing.Size(103, 22);
            this.viewContextMenuStripItem.Text = "&View";
            this.viewContextMenuStripItem.Click += new System.EventHandler(this.viewContextMenuStripItem_Click);
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(100, 6);
            // 
            // editContextMenuStripItem
            // 
            this.editContextMenuStripItem.Name = "editContextMenuStripItem";
            this.editContextMenuStripItem.Size = new System.Drawing.Size(103, 22);
            this.editContextMenuStripItem.Text = "&Edit";
            this.editContextMenuStripItem.Click += new System.EventHandler(this.editContextMenuStripItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(100, 6);
            // 
            // closeContextMenuStripItem
            // 
            this.closeContextMenuStripItem.Name = "closeContextMenuStripItem";
            this.closeContextMenuStripItem.Size = new System.Drawing.Size(103, 22);
            this.closeContextMenuStripItem.Text = "&Close";
            this.closeContextMenuStripItem.Click += new System.EventHandler(this.closeContextMenuStripItem_Click);
            // 
            // openContextMenuItem
            // 
            this.openContextMenuItem.Name = "openContextMenuItem";
            this.openContextMenuItem.Size = new System.Drawing.Size(103, 22);
            this.openContextMenuItem.Text = "&Open";
            this.openContextMenuItem.Visible = false;
            // 
            // panel2
            // 
            this.panel2.Controls.Add(this.label1);
            this.panel2.Controls.Add(this.projectsComboBox);
            this.panel2.Dock = System.Windows.Forms.DockStyle.Top;
            this.panel2.Location = new System.Drawing.Point(0, 0);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(736, 27);
            this.panel2.TabIndex = 0;
            // 
            // label1
            // 
            this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(488, 3);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(80, 13);
            this.label1.TabIndex = 1;
            this.label1.Text = "View by Project";
            // 
            // projectsComboBox
            // 
            this.projectsComboBox.Dock = System.Windows.Forms.DockStyle.Right;
            this.projectsComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.projectsComboBox.FormattingEnabled = true;
            this.projectsComboBox.Location = new System.Drawing.Point(574, 0);
            this.projectsComboBox.Name = "projectsComboBox";
            this.projectsComboBox.Size = new System.Drawing.Size(162, 21);
            this.projectsComboBox.TabIndex = 0;
            this.projectsComboBox.SelectedIndexChanged += new System.EventHandler(this.projectsComboBox_SelectedIndexChanged);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.newIssueToolStripButton,
            this.newProjectToolStripButton,
            this.toolStripSeparator1,
            this.manageProjectsToolStripButton});
            this.toolStrip1.Location = new System.Drawing.Point(0, 24);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.Size = new System.Drawing.Size(736, 25);
            this.toolStrip1.TabIndex = 4;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // newIssueToolStripButton
            // 
            this.newIssueToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newIssueToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("newIssueToolStripButton.Image")));
            this.newIssueToolStripButton.ImageTransparentColor = System.Drawing.Color.Black;
            this.newIssueToolStripButton.Name = "newIssueToolStripButton";
            this.newIssueToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.newIssueToolStripButton.Text = "New Issue";
            this.newIssueToolStripButton.Click += new System.EventHandler(this.newIssueToolStripButton_Click);
            // 
            // newProjectToolStripButton
            // 
            this.newProjectToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.newProjectToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("newProjectToolStripButton.Image")));
            this.newProjectToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.newProjectToolStripButton.Name = "newProjectToolStripButton";
            this.newProjectToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.newProjectToolStripButton.Text = "New Project";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // manageProjectsToolStripButton
            // 
            this.manageProjectsToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.manageProjectsToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("manageProjectsToolStripButton.Image")));
            this.manageProjectsToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.manageProjectsToolStripButton.Name = "manageProjectsToolStripButton";
            this.manageProjectsToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.manageProjectsToolStripButton.Text = "Manage Projects";
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(736, 380);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.MainMenuStrip = this.menuStrip1;
            this.Name = "MainForm";
            this.Text = "BugFSK";
            this.WindowState = System.Windows.Forms.FormWindowState.Maximized;
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.panel1.ResumeLayout(false);
            this.listViewContextMenuStrip.ResumeLayout(false);
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem fileToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem newToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem projectToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem issueToolStripMenuItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.ListView issuesListView;
        private System.Windows.Forms.ColumnHeader idColumnHeader;
        private System.Windows.Forms.ColumnHeader typeColumnHeader;
        private System.Windows.Forms.ColumnHeader titleColumnHeader;
        private System.Windows.Forms.ColumnHeader priorityColumnHeader;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox projectsComboBox;
        private System.Windows.Forms.ToolStripMenuItem editToolStripMenuItem;
        private System.Windows.Forms.ContextMenuStrip listViewContextMenuStrip;
        private System.Windows.Forms.ToolStripMenuItem viewContextMenuStripItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem editContextMenuStripItem;
        private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
        private System.Windows.Forms.ToolStripMenuItem closeContextMenuStripItem;
        private System.Windows.Forms.ColumnHeader statusColumnHeader;
        private System.Windows.Forms.ToolStripMenuItem openContextMenuItem;
        private System.Windows.Forms.ToolStripMenuItem manageProjectsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem4;
        private System.Windows.Forms.ToolStripMenuItem helpToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem aboutToolStripMenuItem;
        private System.Windows.Forms.ToolStrip toolStrip1;
        private System.Windows.Forms.ToolStripButton newIssueToolStripButton;
        private System.Windows.Forms.ToolStripButton newProjectToolStripButton;
        private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
        private System.Windows.Forms.ToolStripButton manageProjectsToolStripButton;

    }
}

