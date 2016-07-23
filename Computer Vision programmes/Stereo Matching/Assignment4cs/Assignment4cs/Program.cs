using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
//using System.Windows.Media;
using System.Diagnostics;
using OpenCvSharp;
//using OpenCvSharp.CPlusPlus;

namespace Assignment4cs
{
    class Program
    {
        static void Main(string[] args)
        {
            Stopwatch watch = new Stopwatch();
            watch.Start();   
            int windowHeight = 4;
            int maxDisparity = 15;
            int minDisparity = 2;
            Console.WriteLine("Reading image...");
            double[,] left = readInput("im2.png");
            double[,] right = readInput("im6.png");
            Console.WriteLine("Finished reading image");
            Console.WriteLine("Started stereo matching...");
            var disparityMap = DisparityMap(left, right, minDisparity, maxDisparity, windowHeight);
            Console.WriteLine("Finished stereo mathcing...");
            Console.WriteLine("Start making disparity map image...");
            var bitmap = createImage(disparityMap, minDisparity, maxDisparity,0);
            bitmap.Save("disparityMap.png");
            watch.Stop();
            Console.WriteLine("Finished in time: {0}:{1}:{2}",watch.Elapsed.Minutes,watch.Elapsed.Seconds,watch.Elapsed.Milliseconds);
            watch.Reset();
            watch.Start();
            Console.WriteLine("Starting Dynamic Programming Stereo Matching...");
            var dynamicDisparity = getDynamic(left, right);
            dynamicDisparity.Save("disparityMapDP.png");
            watch.Stop();
            Console.WriteLine("Finished in time: {0}:{1}:{2}", watch.Elapsed.Minutes.ToString("00"), watch.Elapsed.Seconds.ToString("00"), watch.Elapsed.Milliseconds.ToString("00"));
            Console.ReadLine();

        }

        //A dynamic programming method to calculate disparity, Much faster than the local window approach.
        private static Bitmap getDynamic(double[,] left, double[,] right)
        {
            double max = 0;
            double min = 1000;
            int height = left.GetLength(0);
            int width = left.GetLength(1);
            double[,] result = new double[height, width];
            double[,] intensityMap = new double[width, width];
            for (int i = 0; i < width; i++)
            {
                for (int j = 0; j < width; j++)
                {
                    intensityMap[i, j] = 1;
                }
            }
            for (int y = 0; y < height; y++)
            {
                for (int i = 1; i < width; i++)
                {
                    for (int j = 1; j < width; j++)
                    {    // scanline loop
                        double rightI = right[y, i - 1];
                        double leftI = left[y, j - 1];   // scanline
                        double difI = Math.Abs(leftI - rightI);
                        double m00 = intensityMap[i - 1, j - 1];
                        double m01 = intensityMap[i - 1, j];
                        double m10 = intensityMap[i, j - 1];
                        double add = Math.Min(m00, m01);
                        add = Math.Min(add, m10);
                        add += difI;
                        intensityMap[i, j] = add;
                    }
                }
                // Initialize paths to width;
                int pathx = width -1;
                int pathy = width -1;
                while (pathx > 0 && pathy > 0)
                {
                    //Set 3 variables to infinity (large number)
                    double d00 = 10000;
                    double d01 = 10000;
                    double d10 = 10000;
                    //Case basis
                    if (pathx > 0 && pathy > 0) d00 = intensityMap[pathy - 1, pathx - 1];
                    if (pathx > 0) d01 = intensityMap[pathy, pathx - 1];
                    if (pathy > 0) d10 = intensityMap[pathy - 1, pathx];
                    //Setting new position
                    if (d00 <= d01 && d00 <= d10)
                    {
                        pathx--;
                        pathy--;
                    }
                    else if (d01 < d00 && d01 <= d10)
                    {
                        pathx--;
                    }
                    else {
                        pathy--;
                    }
                    double disp = Math.Abs(pathy - pathx);

                    max = Math.Max(max, disp);
                    min = Math.Min(min, disp);
                    result[y, pathx] = disp;
                }
            }
            //Console.WriteLine(min);
            //Console.WriteLine(max);
            //min = Math.Abs(min);
            //for(int i = 0; i < height; i++)
            //{
            //    for(int j = 0; j < width; j++)
            //    {
            //        result[i, j] = result[i, j] + min;
            //    }
            //}
            //max = max + min;
            //Console.WriteLine("Max: {0}, Min: {1}", max, min);
            Console.WriteLine("Finished Dynamic Programming");
            Console.WriteLine("Start making disparity map image...");
            var bitmap2 = createImage(result,0, (int)max,255);
            return bitmap2;
        }

        //Method to take the disparity map 2d array and write to an image for display purposes.
        private static Bitmap createImage(double[,] disparityMap, int minDisp, int maxDisp, int swap)
        {
            int width = disparityMap.GetLength(1);
            int height = disparityMap.GetLength(0);
            Bitmap result = new Bitmap(width, height);
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    int greyscale = (int)(255.0 * ((disparityMap[y, x] - minDisp) / (maxDisp - minDisp)));
                    //if(swap != 0) greyscale = swap - greyscale;
                    Color c = Color.FromArgb(greyscale, greyscale, greyscale);
                    result.SetPixel(x, y, c);
                }
            }
            return result;
        }

        //Loop to go through the complete image. i = height, j = width.
        private static double[,] DisparityMap(double[,] left, double[,] right, int minDisp, int maxDisp, int frame)
        {
            int height = left.GetLength(0);
            int width = left.GetLength(1);
            double[,] disparityMap = new double[height, width];
            for (int i = 0; i < height; i++)
            {
                for (int j = 0; j < width; j++)
                {                  
                    disparityMap[i, j] = getDisparity(left, right, i, j, minDisp, maxDisp, frame)+ minDisp;
                    //Console.WriteLine("i: {0} j: {1} min: {2} max: {3} frame: {4}", i, j, minDisp, maxDisp, frame);
                }
            }
            return disparityMap;

        }

        //Loop to test a pixel in the left image against a pixel in the right image and return a cost stored in an array.
        private static double getDisparity(double[,] left, double[,] right, int y, int x, int minDisp, int maxDisp, int frame)
        {
            double[] cost = new double[maxDisp - minDisp];
            int count = 0;
            for (int i = minDisp; i < maxDisp; i++)
            {
                cost[count] = sumWindows(left, right, y, x, i, frame);
                count++;
            }

            return Array.IndexOf(cost, cost.Min()); //Minimum cost is found and the index returned. Adding minimum disparity gives final disparity.
        }


        //A double loop to test a window in the right image against a window (plus disparity) in the left image.
        private static double sumWindows(double[,] left, double[,] right, int y, int x, int disp, int frame)
        {
            int frameLeft = Math.Max(0, x - frame);
            int frameRight = Math.Min(x + frame, left.GetLength(1));
            int frameTop = Math.Max(0, y - frame);
            int frameBottom = Math.Min(y + frame, left.GetLength(0)); //Variables for when window is at edge of image frame.
            double cost = 0.0;

            for(int i = frameTop; i < frameBottom; i++)
            {
                for(int j = frameLeft; j < frameRight; j++)
                {
                    double rightI = right[i, j];
                    double leftI = left[i, Math.Min(j + disp, right.GetLength(1)-1)];
                    cost = cost + sumSqaures(rightI, leftI); 
                }
            }
            return cost; //A cost is returnes and stored
        }

        //Method used to square the difference of two pixel values (Seperated for easy modification to other function types).
        private static double sumSqaures(double v1, double v2)
        {
            return (v1 - v2)*(v1- v2);
        }

        //Method to read in an image and store pixel intensities into a 2d array
        public static double[,] readInput(string image)
        {

            Bitmap img = new Bitmap(image);
            double[,] imgMatrix = new double[img.Height, img.Width];
            for (int i = 0; i < img.Width; i++)
            {
                for (int j = 0; j < img.Height; j++)
                {
                    Color pixel = img.GetPixel(i, j);
                    double red = pixel.R * 0.4;
                    double green = pixel.G * 0.3;
                    double blue = pixel.B * 0.3; //Assumptions made that green is more important for pixel matching than red than blue. Minimal differences to changing these.
                    imgMatrix[j, i] = red + green + blue;
                }
            }

            return imgMatrix;
        }

        //Unused debugging method
        /*public static void printImage(double[,] avgLeft, double[,] avgRight)
        {
            using (StreamWriter file = new StreamWriter("left.csv"))
            {
                file.WriteLine("Left,Right");
                for (int i = 0; i < avgLeft.GetLength(1); i++)
                {
                    file.Write(avgLeft[100, i]);
                    file.Write(",");
                    file.Write((avgRight[100, i]));
                    file.WriteLine();
                }
            }
        }*/

    }
}
