#ifndef __CL_INPUT_HOTSWAP_H
#define __CL_INPUT_HOTSWAP_H

#define HOTSWAP_ID_WHITE 0
#define HOTSWAP_ID_BLACK 1
#define HOTSWAP_ID_YELLOW 2

#define HOTSWAP_CAT_WEAPON 0
#define HOTSWAP_CAT_ITEM 1024
#define HOTSWAP_CAT_FORCE 2048

class HotSwapManager
{
private:
	bool down;	  //Is the button down?
	bool noBind;  //Don't bind the button.
	int bindTime; //How long the button has been down with the selection up.
	int uniqueID; //Unique ID for this button.

	//Return the binding for the button, or NULL if none.
	const char *GetBinding(void);
	const char *GetBindingUp(void);

	//Returns true if the weapon/force/item select screen is up.
	bool HUDInBindState(void);

	//Returns true if the weapon/force/item select screen is up.
	bool ForceSelectUp(void);
	bool WeaponSelectUp(void);
	bool ItemSelectUp(void);

	//Binds the button based on the current HUD selection.
	void Bind(void);

	//Execute the current bind, if there is one.
	void Execute(void);
	void ExecuteUp(void);

	//Reset the object to the default state.
	void Reset(void);

public:
	HotSwapManager(int uniqueID);

	//Call every frame.  Uses cg.frametime to increment timers.
	void Update(void);

	//Set the button down or up.
	void SetDown(void);
	void SetUp(void);

	//Returns true if the button is currently down.
	bool ButtonDown(void) { return down; }
};

//External bind function for sharing with UI.
extern void HotSwapBind(int buttonID, int category, int value);

#endif
