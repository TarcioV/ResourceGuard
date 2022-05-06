#ifndef __RESOURCE_GUARD_H__
#define __RESOURCE_GUARD_H__

#include <vector>

class Guardable
{
public:
	friend class ResourceGuardChain; 
	
	Guardable() : mFree(false) { }
	virtual ~Guardable() = default;

	Guardable(const Guardable& other) = delete;
	Guardable& operator=(const Guardable& other) = delete;

protected:
	bool mFree;
};

template <class T>
class ResourceGuard : public Guardable
{
public:
	ResourceGuard(T t) : mT(t) { }

	virtual ~ResourceGuard() {
		if(!mFree)
			mT();
	}

	void free() {
		mFree = true;
	}

private:
	T mT;
};

class ResourceGuardChain
{
public:
	ResourceGuardChain() : mSize(0) {
		//
	}
	~ResourceGuardChain() {
		while(!mObjs.empty()) {
			Guardable *g = mObjs.back();
			delete g;
			mObjs.pop_back();
		}
	}

	ResourceGuardChain(const ResourceGuardChain& other) = delete;
	ResourceGuardChain& operator=(const ResourceGuardChain& other) = delete;

	void add(Guardable *g) {
		mObjs.push_back(g);
	}

	void freeAll() {
		for(Guardable *g : mObjs) {
			g->mFree = true;
		}
	}

private:
	int mSize;
	std::vector<Guardable *> mObjs;
};

#define SINGLE_RESOURCE_GUARD(name, lambda) \
	auto _resource_guarad_WA38ax__lambda_##name = lambda; \
	ResourceGuard<decltype(_resource_guarad_WA38ax__lambda_##name)> name(_resource_guarad_WA38ax__lambda_##name);

#define ADD_RESOURCE_GUARD(c, l) 		\
	{ 												\
		auto _resource_guarad__lambda = l;		\
		c.add(new ResourceGuard<decltype(_resource_guarad__lambda)>(_resource_guarad__lambda));		\
	}

#endif // __RESOURCE_GUARD_H__