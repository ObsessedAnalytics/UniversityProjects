using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Drawing;
using System.IO;
using Accord.Math;
using System.Diagnostics;

namespace Rectification
{
    class Program
    {
        static void Main(string[] args)
        {
            double[,] leftHomography = readFile("leftHomography.csv");
            double[,] rightHomography = readFile("rightHomography.csv");
            Bitmap left = new Bitmap("left.jpg");
            Bitmap right = new Bitmap("right.jpg");
            Bitmap leftRect = new Bitmap(left.Width, left.Height);
            Bitmap rightRect = new Bitmap(right.Width, right.Height);

            Stopwatch watch = new Stopwatch();
            watch.Start();

            //Rectify left image
            for(int i = 0; i < leftRect.Height; i++)
            {
                for(int j = 0; j < leftRect.Width; j++)
                {
                    double[] p = new double[3];
                    double[] q = new double[] { (double)j, (double)i, 1.0 };
                    p = Accord.Math.Matrix.Multiply(leftHomography, q);
                    var newP = new double[] { p[0] / p[2], p[1] / p[2] };
                    if(newP[0] > 0 && newP[0] < left.Width && newP[1] > 0 && newP[1] < left.Height)
                    {
                        Color pixel = left.GetPixel((int)newP[0], (int)newP[1]);
                        leftRect.SetPixel(j, i, pixel);
                    }
                }
            }
            // Save left image

            leftRect.RotateFlip(RotateFlipType.Rotate180FlipNone);
            leftRect.Save("left_rect.jpg");

            for (int i = 0; i < rightRect.Height; i++)
            {
                for (int j = 0; j < rightRect.Width; j++)
                {
                    double[] p = new double[3];
                    double[] q = new double[] { (double)j, (double)i, 1.0 };
                    p = Accord.Math.Matrix.Multiply(rightHomography, q);
                    var newP = new double[] { p[0] / p[2], p[1] / p[2] };
                    if (newP[0] > 0 && newP[0] < right.Width && newP[1] > 0 && newP[1] < right.Height)
                    {
                        Color pixel = right.GetPixel((int)newP[0], (int)newP[1]);
                        rightRect.SetPixel(j, i, pixel);
                    }
                }
            }

            rightRect.RotateFlip(RotateFlipType.Rotate180FlipNone);
            rightRect.Save("right_rect.jpg");
            watch.Stop();

            Console.WriteLine(watch.Elapsed);
            Console.ReadLine();
            

        }


        private static double[,] readFile(string name)
        {
            var reader = new StreamReader(File.OpenRead(name));
            double[,] result = new double[3, 3];
            int count = 0;
            while (!reader.EndOfStream)
            {
                string tokens = reader.ReadLine();
                string[] values = tokens.Trim().Split(',');
                result[count, 0] = Double.Parse(values[0]);
                result[count, 1] = Double.Parse(values[1]);
                result[count, 2] = Double.Parse(values[2]);
                count++;
            }

            return result;
        }

    }


}
