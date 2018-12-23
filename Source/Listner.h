#ifndef LISTNER_H
#define LISTNER_H

#include <vector>
class ICallBack;
class Listner
{
public:
	void addCallBack(ICallBack* callBack);
	void removeCallBack(const ICallBack* callBack);
	void eventTriggered();
private:
	std::vector<ICallBack*> callBacks;
};
#endif // !LISTNER_H
