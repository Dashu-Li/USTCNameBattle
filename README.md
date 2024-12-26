# USTC名字大作战

## 简介
USTC名字大作战是一个基于C++和Qt框架开发的游戏。玩家通过名字进行大作战，每个名字代表一个玩家，通过攻击、防御、治疗等操作来进行对战。

## 游戏逻辑
游戏中的主要逻辑包括：
- **玩家属性**：每个玩家有生命值、攻击力、防御力、暴击率、闪避率、恢复力等属性。
- **战斗系统**：游戏通过权重随机生成攻击者和防御者，计算伤害，进行生命值和经验值的更新。
- **技能系统**：包括普通攻击、治疗、吸血、飞升等技能。

## UI界面设计
游戏的UI界面主要设计如下：
- **按钮**：
  - 开始游戏按钮：用于开始游戏。
  - 快速模式按钮：加快游戏进程。
  - 再玩一次按钮：重新开始游戏。
- **文本框**：
  - `battleEdit`：显示对战过程信息。
- **进度条**：
  - 用于显示玩家的生命值。

### 主要函数功能
以下是代码中主要函数的功能介绍：

#### Game.h
- `Player` 类：管理玩家属性和行为。
  - `Player(std::string name = "")`：构造函数，初始化玩家。
  - `getName()`：获取玩家名字。
  - `getHp()`：获取玩家当前生命值。
  - `setHp(int hp)`：设置玩家生命值。
  - 其他类似的getter和setter函数。

- `Game` 类：管理游戏的核心逻辑。
  - `Game()`：构造函数，初始化游戏。
  - `addPlayer(std::string name, int team)`：添加玩家到指定队伍。
  - `GenerateGame()`：生成对局。
  - `GenerateAttack()`：生成普通攻击动作。
  - `GenerateHeal()`：生成治疗动作。
  - `GenerateLifesteal()`：生成吸血动作。
  - `GenerateAscension()`：生成飞升动作。

#### GameWindow.h
- `GameWindow` 类：管理游戏窗口的显示和交互。
  - `displayStatus()`：显示每个玩家的生命条。
  - `on_battleStartButton_clicked()`：处理开始战斗按钮点击事件。
  - `on_PlayAgainButton_clicked()`：处理再玩一次按钮点击事件。
  - `displayAction(Action* action)`：显示动作信息。
  - `GameEnd(std::vector<Player*>)`：处理游戏结束事件。

#### USTCNameBattle.h
- `USTCNameBattle` 类：管理主窗口和游戏窗口的切换。
  - `on_gameStartButton_clicked()`：显示游戏窗口，隐藏主窗口。
  - `Back()`：显示主窗口，隐藏游戏窗口。

## 结算界面
结算界面会显示每个玩家的最终得分、GPA、击杀数等信息，并通过`battleEdit`文本框展示。
