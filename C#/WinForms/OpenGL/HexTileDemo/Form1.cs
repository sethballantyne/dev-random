/*Copyright (c) 2016 Seth Ballantyne <seth.ballantyne@gmail.com>
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
using OpenTK.Graphics;
using OpenTK.Graphics.OpenGL;
using System.Diagnostics;

namespace HexTileDemo
{
    public partial class Form1 : Form
    {
        bool loaded = false;
        int x;
        int grassTexture;
        int hillTexture;
        int hillTexture32;
        int grassTexture32;
        //FPSCounter fpsCounter;
        Point mapPosition = new Point(0, 0);
        const int numLists = 2;
        int[] lists = new int[numLists];
        int firstList = 0;
        int zoomLevel = 1;
        int[] tileSize = new int[2] { 16, 32 };
        int[,] map = new int[600, 600];
        public Form1()
        {
            InitializeComponent();

            //fpsCounter = new FPSCounter(glControl1, this);

            Random rand = new Random();
            for (int y = 0; y < map.GetLength(0); y++)
            {
                for (int x = 0; x < map.GetLength(1); x++)
                {
                    map[y, x] = rand.Next(2);
                }
            }
            
        }

        private int LoadTexture(string filename)
        {
            int texture = 0;

            using (Bitmap image = new Bitmap(filename))
            {
                image.MakeTransparent(Color.FromArgb(255, 0, 132));

                GL.BlendFunc(BlendingFactorSrc.SrcAlpha, BlendingFactorDest.OneMinusSrcAlpha);
                GL.GenTextures(1, out texture);
                GL.BindTexture(TextureTarget.Texture2D, texture);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMinFilter, (int)TextureMinFilter.Linear);
                GL.TexParameter(TextureTarget.Texture2D, TextureParameterName.TextureMagFilter, (int)TextureMagFilter.Linear);

                System.Drawing.Imaging.BitmapData data = image.LockBits(new System.Drawing.Rectangle(0, 0, image.Width, image.Height),
                    System.Drawing.Imaging.ImageLockMode.ReadOnly, System.Drawing.Imaging.PixelFormat.Format32bppArgb);

                GL.TexImage2D(TextureTarget.Texture2D, 0, PixelInternalFormat.Rgba, data.Width, data.Height, 0,
                    OpenTK.Graphics.OpenGL.PixelFormat.Bgra, PixelType.UnsignedByte, data.Scan0);

                image.UnlockBits(data);
            }

            return texture;
        }

        private void SetupViewport()
        {
            GL.Viewport(0, 0, glControl1.Width, glControl1.Height);

            GL.MatrixMode(MatrixMode.Projection);
            GL.LoadIdentity();
            
            int w = glControl1.Width;
            int h = glControl1.Height;
            
            GL.Ortho(0, w, h, 0, 0.0, 1.0f); // Bottom-left corner pixel has coordinate (0, 0)
            
        }

        private void Render()
        {
            int tileSize = this.tileSize[zoomLevel];
            int quarterOfCurrentTileHeight = tileSize / 4 ;
            Rectangle destRect = new Rectangle();

            GL.Clear(ClearBufferMask.ColorBufferBit);

            for (int i = 0; i < 43; i++)
            {
                for (int j = 0; j < 30; j++)
                {
                    if ((j % 2) == 0)
                    {
                        destRect.X = (i * tileSize) - (tileSize / 2);
                    }
                    else
                    {
                        destRect.X = i * tileSize;
                    }

                    destRect.Y = j * (tileSize - quarterOfCurrentTileHeight) - (tileSize / 2);
                   
                    int tile = map[mapPosition.Y + j, mapPosition.X + i];

                    if (zoomLevel == 0)
                    {

                        switch (tile)
                        {
                            case 0: GL.BindTexture(TextureTarget.Texture2D, grassTexture); break;
                            case 1: GL.BindTexture(TextureTarget.Texture2D, hillTexture); break;
                            default:
                                break;
                        }
                    }
                    else
                    {
                       
                            switch (tile)
                            {
                                case 0: GL.BindTexture(TextureTarget.Texture2D, grassTexture32); break;
                                case 1: GL.BindTexture(TextureTarget.Texture2D, hillTexture32); break;
                                default:
                                    break;
                            }
                        
                    }

                    GL.MatrixMode(MatrixMode.Modelview);
                    GL.LoadIdentity();
                    //System.Diagnostics.Debug.Print("{0} {1}", destRect.X, destRect.Y);
                    GL.Translate(destRect.X, destRect.Y, 0);
                    //GL.BindTexture(TextureTarget.Texture2D, grassTexture);
                    //GL.CallLists(1, ListNameType.Int, lists);
                    GL.CallList(lists[zoomLevel]);
                    //GL.Translate(32, 0, 0);
                    ////GL.BindTexture(TextureTarget.Texture2D, hillTexture);
                    //GL.CallLists(1, ListNameType.Int, lists);
                }
            }
            
            glControl1.SwapBuffers();
        }


        private void glControl1_Load(object sender, EventArgs e)
        {
            loaded = true;

            GL.ClearColor(Color.MidnightBlue);
            GL.Enable(EnableCap.Texture2D);
            GL.Enable(EnableCap.Blend);

            hillTexture = LoadTexture("hill.bmp");
            grassTexture = LoadTexture("grass.bmp");
            grassTexture32 = LoadTexture("grass32.bmp");
            hillTexture32 = LoadTexture("hill32.bmp");

             firstList = GL.GenLists(numLists);

            lists[0] = CreateList(0, 16);
            lists[1] = CreateList(1, 32);

            //for (int i = 0; i < numLists; i++)
            //{
            //    lists[i] = firstList + i;
            //    GL.NewList(firstList + i, ListMode.Compile);
                
            //    GL.Begin(PrimitiveType.Quads);

            //    mouseX = 0;
            //    mouseY = 0;
            //    GL.TexCoord2(0.0f, 0.0f);
            //    GL.Vertex2(0, 0); // top left
            //    GL.TexCoord2(1.0f, 0.0f);
            //    GL.Vertex2(16, 0); // top right
            //    GL.TexCoord2(1.0f, 1.0f);
            //    GL.Vertex2(16, 16); // bottom right
            //    GL.TexCoord2(0.0f, 1.0f);
            //    GL.Vertex2(0, 16); // bottom left
            //    GL.End();

            //    GL.EndList();
            //}

            SetupViewport();
            //fpsCounter.Start();
            glControl1.Invalidate();

            glControl1.Focus();
        }

        private int CreateList(int index, int frameSize)
        {
            int listname = firstList + index;
            GL.NewList(listname, ListMode.Compile);

            GL.Begin(PrimitiveType.Quads);

            GL.TexCoord2(0.0f, 0.0f);
            GL.Vertex2(0, 0); // top left
            GL.TexCoord2(1.0f, 0.0f);
            GL.Vertex2(frameSize, 0); // top right
            GL.TexCoord2(1.0f, 1.0f);
            GL.Vertex2(frameSize, frameSize); // bottom right
            GL.TexCoord2(0.0f, 1.0f);
            GL.Vertex2(0, frameSize); // bottom left
            GL.End();

            GL.EndList();

            return listname;
        }


        private void glControl1_Resize(object sender, EventArgs e)
        {
            if (!loaded)
            {
                return;
            }
            SetupViewport();
            
            glControl1.Invalidate();
        }

        private void glControl1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Space)
                x++;

            glControl1.Invalidate();
        }

        private void glControl1_Paint(object sender, PaintEventArgs e)
        {
            if (loaded)
                Render();
        }

        private void Form1_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Space)
                x++;

            glControl1.Invalidate();
        }

        private void glControl1_MouseDown(object sender, MouseEventArgs e)
        {
            x++;
            glControl1.Invalidate();
        }

        private void glControl1_MouseMove(object sender, MouseEventArgs e)
        {
            
            //this.Text = Convert.ToString(e.Y);
            //mouseX = e.X;
            ////mouseX = e.X;
            //mouseY = e.Y;
        }

        private void glControl1_MouseClick(object sender, MouseEventArgs e)
        {
            if (zoomLevel == 1) zoomLevel--;
            else zoomLevel++;
        }

        private void vScrollBar1_Scroll(object sender, ScrollEventArgs e)
        {
            switch (e.Type)
            {
                case ScrollEventType.LargeIncrement:
                case ScrollEventType.SmallIncrement:
                case ScrollEventType.ThumbPosition:
                case ScrollEventType.ThumbTrack:
                    mapPosition.Y += 2;
                    Render();
                    break;
            }
        }

        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            GL.DeleteTexture(hillTexture);
            GL.DeleteTexture(grassTexture);
        }
    }
}
