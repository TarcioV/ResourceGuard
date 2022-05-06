## Resource Guard

#### Use the fexibility of lambda expressions to prevent resource leak.

### Motivation

To avoid resource leak when porting legacy code most time we have to create several micro classes using [RAII idom](https://en.wikipedia.org/wiki/Resource_acquisition_is_initialization). To prevent the proliferation of these micro classes we could optionally use lambda expressions.

### Example

```
// ...
#include <ResourceGuard.h>

class App
{
public:
	App() {
		ResourceGuardChain algorithms;
	
		if(init_device() != OK) {
			throw std::runtime_error("init_device() failed\n");
		}

		algorithms.add(new ResourceGuard([](){
			close_device();
		}));

		if(open_window() != OK) {
			throw std::runtime_error("open_window() failed\n");
		}

		algorithms.add(new ResourceGuard([](){
			close_window();
		}));

		if(create_opengl_context() != OK) {
			throw std::runtime_error("create_opengl_context() failed\n");
		}

		algorithms.freeAll();
	}
	
	~App() {
		destroy_opengl_context();
		close_window();
		close_device();
	}
};

int main() {
	try {
		App app;
		std::cout << "app successful created\n";
	} catch (std::exception& e) {
		std::cerr << "exception occurs: " << e.what() << '\n';
		return -1;
	}

	return 0;
}
```

one possible output is:

```
trying init device... device initialized
try opening window... window opened
trying create a opengl context... context created
app successful created
```

But if something went wrong and the context couldn't be create, we got:

```
trying init device... device initialized
try opening window... window opened
trying create a opengl context... can't create context
close_window()
close_device()
exception occurs: create_opengl_context() failed
```

Note the destructors were called in the reverse order.

For simple tasks, one could use a single resource guard:

```
App() {
	if(init_device() != OK) {
		throw std::runtime_error("init_device() failed\n");
	}

	ResourceGuard device_guard([](){
		close_device();
	});

	open_window_with_opengl_context();

	device_guard.free();
}
```

### Macro version

When working with c++11 and c++14 you should use the macro version of the library.

```
App() {
	ResourceGuardChain algorithms;

	if(init_device() != OK) {
		throw std::runtime_error("init_device() failed\n");
	}

	ADD_RESOURCE_GUARD(algorithms, [](){
		close_device();
	});
	...
	algorithms.freeAll();
}
```

or single resource guard:

```
App() {
	if(init_device() != OK) {
		throw std::runtime_error("init_device() failed\n");
	}
	
	SINGLE_RESOURCE_GUARD(device_guard, ([](){
		close_device();
	});

	open_window_with_opengl_context();
	
	device_guard.free();
}
```

### Build

Resource Guard is a header only file. Just download ```ResourceGuard.h``` and place it in a folder your compile can find.

### Compatibility

- Macro version should works with all c++11 and latter compliance compilers.
- Class version should works with all c++17 and latter compliance compilers. One exception is vs2015 because it doesn't fully support all c++17 features.