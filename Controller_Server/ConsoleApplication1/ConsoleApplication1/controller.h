#pragma once
class controller
{
public:
	int controllerType;
	//write the data to the struct (in deriv classes)
	//return: if it worked
	virtual bool updateData() = 0;

	//send data (set it with setters)
	virtual bool sendData() = 0;

private:
	controller();
	~controller();
};

