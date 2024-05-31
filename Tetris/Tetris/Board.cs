using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Tetris
{
    class Board
    {
        private int rows;
        private int columns;
        private int score;
        private int level;

        public int[,] grid = new int[10, 20];

        public Board()
	    {
	         
	    }
    }
}