#pragma once
#include <memory>
class GameMain;
class SequenceFrame
{
public:
	virtual ~SequenceFrame(){};
	virtual std::shared_ptr<SequenceFrame> update(const GameMain& game) = 0;
private:

};

