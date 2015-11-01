#ifndef _SQUAD_H
#define _SQUAD_H

#include <include/Minion.hpp>

class Squad {
protected:
	std::vector<Minion> mMembers;
public:
	Squad();
	Squad(int vectorReserve);
	~Squad();

	unsigned int getSize() const;

	void recruit(Minion&& newMember);
	virtual void update(sf::Time const &elapsedTime);
};
#endif