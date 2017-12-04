#pragma once

#include<vector>
using std::vector;
using std::pair;


struct VCPoint  // VC攻击时的攻击点，由活4，冲4，跳冲4，连3，跳3，组成，优先级依次递减
{
	int row;
	int col;
	int pri;  // 优先级
	friend bool operator < (const VCPoint &a,const VCPoint &b)
	{
		return a.pri > b.pri;
	}
	void init(int _r,int _c,int _p)
	{
		row = _r, col = _c, pri = _p;
	}
};

class FiveChess
{
public:
	FiveChess();
	virtual ~FiveChess();

	int nCount; //记录下了多少步棋
	
	void InitChessMap(); //初始化棋盘
	void SetKinjite(bool isKinjite); //为true表示有禁手，为false表示无禁手

	char GetStatus(int row,int col);  //得到chessMap[row][col]的值
	void SetCurrentPoint(int row,int col); //设置当前下落点

	void SetChess(int row,int col,char chessFlag);  //下一步棋
	void RetractChess(); //悔一步棋

	char JudgeWin(char chessFlag); //判断chessFlag色的棋子是否赢棋

public:
	vector< pair<int,int> > chessPro; //保存下棋过程

	char chessMap[SIZE][SIZE]; //棋盘
	int currentX,currentY; //当前落子点

	bool haveKinjite;  //该盘棋是否设置 有禁手,（实际上交给上一层去判断，这里只有人机的AI需要这个标记）


	//下面是 AI 模块
private:

	//记录未下过的点对于计算机和人的价值
	int computer[SIZE][SIZE],people[SIZE][SIZE];

	int peoX,peoY;  // AI中人下这一点

//	根据五子棋的特点,可以产生一个棋面限制框,记录当前棋面所有棋子的最左、
//	最右、最上、最下点构成的矩形,而且基本可以认为下一步棋的位置不会脱离这个框MM(MM自己设置)步以上.
	int left,right,top,bottom;

	//对某一位置能产生各种棋型数量的统计
	int changlian,cheng5,huo4,chong4,tiaochong4,lian3,tiao3,chong3,lian2,tiao2; //
	int chong2,huo1,chong1;

	// 下面几个函数 计算 在 chessMap[row][col] 落子后 会产生的 各棋型的数量 

     /*********************************************************************/
	// 假设在chessMap[row][col] 落子后,是否成5（或长连),返回1表示五连，返回2表示长连,否则返回0 
	int GetCheng5ChangLian(int row,int col,char chessFlag);

	//  假设在chessMap[row][col] 落子后，返回构成活4的数量 
	int GetHuo4(int row,int col,char chessFlag);
	
	// 假设在chessMap[row][col] 落子后，返回构成冲3和冲4(chong4和tiaochong4)的数量 
	pair<int,pair<int,int> > GetChong3Chong4(int row,int col,char chessFlag);

	// 假设在chessMap[row][col] 落子后，返回构成连2和连3的数量 
	pair<int,int> GetLian2Lian3(int row,int col,char chessFlag);

	// 假设在chessMap[row][col] 落子后，返回构成跳2和跳3的数量 
	pair<int,int> GetTiao2Tiao3(int row,int col,char chessFlag);

	// 假设在chessMap[row][col] 落子后，判断一些小棋型的数量
	void OntherChessType(int row,int col,char chessFlag);

	/*********************************************************************/

	//搜索 chessMap[row][col] 这点能够针对 chessFlag 色的棋能够构成哪些棋型
	void Search(int row,int col,char chessFlag);  
    
    // 记录是否有必赢点,为1表示一步赢棋（成五），为2表示两步赢棋（例如活4），
	// 为3表示三步赢棋（例如两个活3）,为 0 表示没有必赢点
	int mustWin;   //记录是否有必赢点
	int winPeople,winComputer;  //同样是记录必赢点，记录是谁的必赢点

	//更新棋面限制框
	void UpdateLimit();
	

    //得到 chessMap[row][col] 这点对于 chessFlag 色的棋的价值
	int GetScoreForComputer(int row,int col,char chessFlag); //站在计算机的角度
	int GetScoreForPeople(int row,int col,char chessFlag);   //站在人的角度

	// 上面两个函数的辅助函数，flag为 cComputer 表示站在计算机的角度，为 cPeople表示站在人的角度
	int GetScore(int row,int col,char chessFlag,char flag);

	//得到并返回当前价值最大点，chessFlag 为 cComputer表示站在计算机的角度，为cPeople表示站在人的角度
	pair<int,int> GetCurrentMaxPoint(char chessFlag); 	

	//所谓防守要就要防对方的进攻方向，那么就需要通过加分来实现，在GetScore()中调用
	int AddScore(int row,int col,char chessFlag); 
	int isDefend; //是否防守，在 VCAttack()中决定，在防止对方VC攻击时使用，用在 GetScore()对 AddScore()的调用

	// GameTree的参数中，flag分别表示计算机是否找到必赢点，
	// 若有必赢点，则depthC,depthP表示(计算机，人)必赢点的最小深度
	// 这个函数只是初步筛选
	// InitGameTree（） 和 AgainGameTree（) 的函数体差不多，一个用于计算机的，一个用于人的。
	void InitGameTree(int row,int col,bool& flag,int depth,int& depthC,int& depthP,bool& stop,int depthRecord); 
	
	bool PeopleAttack(int r,int c,int depthCFromIGT); //模拟人攻击
	void AgainGameTree(int row,int col,int depth,int& depthC,int& depthP,bool& stop,int& depthRecord); 
	
	bool ComputerAttack(int r,int c,int depthMM); // 模拟计算机攻击

	bool VCAttack(); // 判断是否可以进行VC攻击
	int IsCombForVC(int row,int col,char chessFlag); // 是否构成可以VC攻击的材料，活4，活3或冲4
	// type 为1 表示 VCF (冲4)攻击，为2表示 VCF和VCT联合攻击
	void VCAttackTree(int type,int row,int col,char cOneself,char cOpposite,int depth,bool& flag,int& ansDepth,int depthRecord); // VC 攻击树

	int DEPTH; // 由 AI 动态控制 InitGameTree 的搜索深度
	void ControlDepth();   // 判断局势，从而设置 DEPTH
	bool LayOut();   // 开始的时候布局

public:
	char cComputer;  //计算机执的棋的颜色
	char cPeople;    //人执的棋的颜色

	int grade; // AI的等级，在 CAIThread 中的 AI()中改变

	int comX,comY; //AI中计算机下这一点

	//黑方在落下的关键的第五子即形成五连的同时，又形成禁手，此时禁手失效，黑棋胜
    //返回1说明这点是黑棋的长连禁手，返回2说明是三三或四四禁手，否则不是禁手
	int IsKinjite(int row,int col); 
	void ComputerPlay();  // 得到 comX,comY
	void PeoplePlay();   //得到 peoX,peoY

	void AI();   //

	int depthMM; // 深度控制

	int VCDEPTH;  // AI 进行 VC 攻击的开始深度
	int VCDEPTH_END;  // AI 进行VC攻击的最高深度
	int VCDEPTH_DIS;  // AI 进行 VC 攻击的深度 递增值
	
	int VCDE_DEPTH;  // AI 进行 VC 防御的开始深度
	int VCDE_DEPTH_END; // AI 进行 VC 防御的最高深度
	int VCDE_DEPTH_DIS; // AI 进行 VC 防御的深度 递增值

	// 下面是攻击状态，可以显示到界面方便用户知道AI目前的状况
	/*
		0 : 表示屁都没有
		1 ：表示计算机正在进行 VCF 攻击
		2 ：表示计算机正在阻止人进行 VCF 攻击
		3 ：表示计算机正在进行 VCF 和 VCT 联合攻击
	*/
	int AIState;  // 在 VCAttack() 中设置

};


/*
      chessMap[][]为 SPACE 表示该位置为空，为 BLACK_CHESS 表示为黑棋，
	  为WHITE_CHESS 表示为白棋。

	  JudgeWin(char chessFlag)返回 chessFlag 说明chessFlag色的棋赢了，
	  返回 SPACE 表示尚未分出胜负，返回 PEACE_CHESS，表示和棋。


  ******   下面是棋型的介绍和禁手规则：
  
	1、 五连：在棋盘上形成的5个同色棋子的"连"。 
	2、 长连：在棋盘上形成的6个或6个以上同色棋子的"连"。
	
	"四"包括"活四"和"冲四"。（为了提高VC攻击能力还分了一个"跳冲四"） 
	1、活四：在棋盘某一条阳线或阴线上有同色4子不间隔地紧紧相连，且在此4子两端延长线上各有一个无子的交叉点与此4子紧密相连。 
	2、冲四：除"活四"外的，再下一着棋便可形成五连，并且存在五连的可能性的局面。 
	3、 白棋再下一着可形成长连的局面也视为"四"。 
	
	"三"指活三，包括"连三"和"跳三"。 
	1、连三：在棋盘某一条阳线或阴线上有同色三子相连，且在此三子两端延长线上有一端至少有一个，另一端至少有两个无子的交叉点与此三子紧密相连。 
	2、跳三：中间仅间隔一个无子交叉点的连三，但两端延长线均至少有一个无子的交叉点与此三子相连。 
	
	禁手：对局中禁止使用的着法。
	  
	
	  1、黑棋禁手包括"三三"、"四四"和"长连"。 
		  1.1 三三：由于黑方走一着在无子交叉点上同时形成二个或二个以上黑方"活"的局面。 
					（连三，跳三 都算）
		  1.2 四四：由于黑方走一着在无子交叉点上同时形成二个或二个以上黑方"四"的局面。 
					（活四，冲四 都算）

	  2、白棋无禁手。
	  
	  "四三"：指某一方同时具备两个先手，其中一个是"四"，一个是"活三"。 

	  先手：对方必须应答的着法，其中"冲四"称为绝对先手。 

	  三手可交换：是指黑棋下盘面第3着棋后，白方在应白④之前，如感觉黑方棋形不利于己方，
	  可提出交换，即执白棋一方变为执黑棋一方，而黑方不可以不换。 

	  五手两打：是指黑棋在下盘面上关键的第5手棋时，必须下两步棋，让白棋在这两步棋中任选一步，
	  然后再继续对弈。一般说来，白棋肯定拿掉对白方不利的一点，而保留对黑方较为不利的那点让黑方行棋。 
		  
	   五连与禁手同时形成，先五为胜。

 ***** 由于 禁手规则判断非常复杂，这里只处理一重禁手，对于多重禁手程序不予判断，由玩家自行判断。
	话说 QQ 游戏的五子棋的禁手判断也有bug, 哥昨儿就逮着一个bug。

     
	   五子棋专业术语：VCF，VCT攻击
	   VCF: victory for continual four  ,持续进行活四，冲四（绝对先手）攻击
	   VCT: victory for continual three, 持续进行活三攻击
	   二者综合称为 VC 攻击
 */