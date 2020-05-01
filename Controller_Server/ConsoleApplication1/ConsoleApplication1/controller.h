#pragma once
class controller
{
public:
	int controllerType;
	//write the data to the struct (in deriv classes)
	//return: if it worked
	virtual bool updateData();

	//send data (set it with setters)
	virtual bool sendData();

private:
	controller();
	~controller();
};

