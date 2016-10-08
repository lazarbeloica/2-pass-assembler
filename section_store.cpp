#include "section_store.h"

bool SectionStore::exists(string name)
{
	map<string, Section*>::iterator it;
	it = store.find(name);

	return it == store.end() ? false : true;
}

void SectionStore::put(Section *s)
{
	if(!exists(s->getName()))
		store.insert(pair<string, Section*>(s->getName(), s));
}

Section * SectionStore::get(string name)
{
	int pos = exists(name);
	return !pos ? 0 : store[name];
}

string SectionStore::toString()
{
	stringstream ss;

	map<string, Section*>::iterator it;
	for (it = store.begin(); it != store.end(); it++)
		ss << it->second->toString() << "------------------------------------------"<< endl;
	
	return ss.str();
}
