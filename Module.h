#ifndef __MODULE_H__
#define __MODULE_H__

class Application;

//General Structure of a module. Init, Start, Pre/-/Post and Cleanup need to be Virtual for the modules to choose if they implement it or not.
class Module
{
public:

	Module(bool active = true) : active(active)
	{}

	virtual ~Module()
	{}

	bool IsEnabled() const
	{
		return active;
	}

	bool Enable()
	{
		if(active == false)
			return active = Start();

		return true;
	}

	bool Disable()
	{
		if(active == true)
			return active = !CleanUp();

		return true;
	}

	virtual void Init() 
	{}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update()
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate()
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

private:
	bool active = true;
};

#endif // __MODULE_H__