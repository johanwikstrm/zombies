#ifndef CELL_H
#define CELL_H value



class Cell
{
public:
	Cell(int kind);
	~Cell();
	int kind();
	void setKind(int);
	bool moveFlag();
	void setMoveFlag(bool);
private:
	int k;
	bool mFlag;
};

#endif