

class AI
{
    // 15*15共有572种五子连珠的可能性
    const int MaxFiveChainCount = 572;

    //玩家的可能性
    bool[, ,] _ptable = new bool[Board.CrossCount, Board.CrossCount, MaxFiveChainCount];

    //电脑的可能性
    bool[, ,] _ctable = new bool[Board.CrossCount, Board.CrossCount, MaxFiveChainCount];

    //记录2位玩家所有可能的连珠数，-1则为永远无法5连珠
    int[,] _win = new int[2, MaxFiveChainCount];

    //记录每格的分值
    int[,] _cgrades = new int[Board.CrossCount, Board.CrossCount];
    int[,] _pgrades = new int[Board.CrossCount, Board.CrossCount];

    //记录棋盘
    int[,] _board = new int[Board.CrossCount, Board.CrossCount];

    int _cgrade, _pgrade;
    int _icount, _m, _n;
    int _mat, _nat, _mde, _nde;

    public AI()
    {
        for (int i = 0; i < Board.CrossCount; i++)
        {
            for (int j = 0; j < Board.CrossCount; j++)
            {
                _pgrades[i, j] = 0;
                _cgrades[i, j] = 0;
                _board[i, j] = 0;
            }
        }

        //遍历所有的五连子可能情况的权值  
        //横  
        for (int i = 0; i < Board.CrossCount; i++)
        {
            for (int j = 0; j < Board.CrossCount - 4; j++)
            {
                for (int k = 0; k < BoardModel.WinChessCount; k++)
                {
                    _ptable[j + k, i, _icount] = true;
                    _ctable[j + k, i, _icount] = true;
                }

                _icount++;
            }
        }

        //横  
        for (int i = 0; i < Board.CrossCount; i++)
        {
            for (int j = 0; j < Board.CrossCount - 4; j++)
            {
                for (int k = 0; k < BoardModel.WinChessCount; k++)
                {
                    _ptable[i, j + k, _icount] = true;
                    _ctable[i, j + k, _icount] = true;
                }

                _icount++;
            }
        }

        // 右斜
        for (int i = 0; i < Board.CrossCount - 4; i++)
        {
            for (int j = 0; j < Board.CrossCount - 4; j++)
            {
                for (int k = 0; k < BoardModel.WinChessCount; k++)
                {
                    _ptable[j + k, i + k, _icount] = true;
                    _ctable[j + k, i + k, _icount] = true;
                }

                _icount++;
            }
        }

        // 左斜
        for (int i = 0; i < Board.CrossCount - 4; i++)
        {
            for (int j = Board.CrossCount - 1; j >= 4; j--)
            {
                for (int k = 0; k < BoardModel.WinChessCount; k++)
                {
                    _ptable[j - k, i + k, _icount] = true;
                    _ctable[j - k, i + k, _icount] = true;
                }

                _icount++;
            }
        }

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < MaxFiveChainCount; j++)
            {
                _win[i, j] = 0;
            }
        }

        _icount = 0;
    }




    void CalcScore()
    {
        _cgrade = 0;
        _pgrade = 0;
        _board[_m, _n] = 1;//电脑下子位置     

        for (int i = 0; i < MaxFiveChainCount; i++)
        {
            if (_ctable[_m, _n, i] && _win[0, i] != -1)
            {
                _win[0, i]++;//给白子的所有五连子可能的加载当前连子数  
            }

            if (_ptable[_m, _n, i])
            {
                _ptable[_m, _n, i] = false;
                _win[1, i] = -1;
            }
        }
    }

    void CalcCore()
    {
        //遍历棋盘上的所有坐标  
        for (int i = 0; i < Board.CrossCount; i++)
        {
            for (int j = 0; j < Board.CrossCount; j++)
            {
                //该坐标的黑子奖励积分清零  
                _pgrades[i, j] = 0;

                //在还没下棋子的地方遍历  
                if (_board[i, j] == 0)
                {
                    //遍历该棋盘可落子点上的黑子所有权值的连子情况，并给该落子点加上相应奖励分  
                    for (int k = 0; k < MaxFiveChainCount; k++)
                    {
                        if (_ptable[i, j, k])
                        {
                            switch (_win[1, k])
                            {
                                case 1://一连子  
                                    _pgrades[i, j] += 5;
                                    break;
                                case 2://两连子  
                                    _pgrades[i, j] += 50;
                                    break;
                                case 3://三连子  
                                    _pgrades[i, j] += 180;
                                    break;
                                case 4://四连子  
                                    _pgrades[i, j] += 400;
                                    break;
                            }
                        }
                    }

                    _cgrades[i, j] = 0;//该坐标的白子的奖励积分清零  
                    if (_board[i, j] == 0)//在还没下棋子的地方遍历  
                    {
                        //遍历该棋盘可落子点上的白子所有权值的连子情况，并给该落子点加上相应奖励分  
                        for (int k = 0; k < MaxFiveChainCount; k++)
                        {
                            if (_ctable[i, j, k])
                            {
                                switch (_win[0, k])
                                {
                                    case 1://一连子  
                                        _cgrades[i, j] += 5;
                                        break;
                                    case 2: //两连子  
                                        _cgrades[i, j] += 52;
                                        break;
                                    case 3://三连子  
                                        _cgrades[i, j] += 130;
                                        break;
                                    case 4: //四连子  
                                        _cgrades[i, j] += 10000;
                                        break;
                                }
                            }
                        }

                    }


                }
            }
        }

    }

    // AI计算输出, 需要玩家走过的点
    public void ComputerDo(int playerX, int playerY, out int finalX, out int finalY)
    {
        setPlayerPiece(playerX, playerY);

        CalcCore();

        for (int i = 0; i < Board.CrossCount; i++)
        {
            for (int j = 0; j < Board.CrossCount; j++)
            {
                //找出棋盘上可落子点的黑子白子的各自最大权值，找出各自的最佳落子点 
                if (_board[i, j] == 0)
                {
                    if (_cgrades[i, j] >= _cgrade)
                    {
                        _cgrade = _cgrades[i, j];
                        _mat = i;
                        _nat = j;
                    }

                    if (_pgrades[i, j] >= _pgrade)
                    {
                        _pgrade = _pgrades[i, j];
                        _mde = i;
                        _nde = j;
                    }

                }
            }
        }

        //如果白子的最佳落子点的权值比黑子的最佳落子点权值大，则电脑的最佳落子点为白子的最佳落子点，否则相反  
        if (_cgrade >= _pgrade)
        {
            _m = _mat;
            _n = _nat;
        }
        else
        {
            _m = _mde;
            _n = _nde;
        }


        CalcScore();

        finalX = _m;
        finalY = _n;
    }

    void setPlayerPiece(int playerX, int playerY)
    {
        int m = playerX;
        int n = playerY;

        if (_board[m, n] == 0)
        {
            _board[m, n] = 2;

            for (int i = 0; i < MaxFiveChainCount; i++)
            {
                if (_ptable[m, n, i] && _win[1, i] != -1)
                {
                    _win[1, i]++;
                }
                if (_ctable[m, n, i])
                {
                    _ctable[m, n, i] = false;
                    _win[0, i] = -1;
                }
            }
        }
    }

}

