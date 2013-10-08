#ifndef CELL_H
#define CELL_H value



class Cell
{
public:
	Cell(int kind);
        Cell(const Cell& c);
	~Cell();
	int kind();
	void setKind(int);
	bool moveFlag();
	void setMoveFlag(bool);
	Cell * Spawn(int newKind);
private:
	int k;
	bool mFlag;
};

#endif
