extends Control

func _process(_delta: float) -> void:
	# key just pressed
	if GlobalInput.is_global_key_just_pressed(KEY_G): 
		print("G key just pressed")
		
	# key held down
	if GlobalInput.is_global_key_pressed(KEY_J):
		print("J key held down")
	
	# key just released
	if GlobalInput.is_global_key_just_released(KEY_J):
		print("J key just released")
	
	# mouse just pressed
	if GlobalInput.is_global_mouse_button_just_pressed(MOUSE_BUTTON_MIDDLE):
		print("Middle mouse btn just pressed")
	
	# mouse held down
	if GlobalInput.is_global_mouse_button_pressed(MOUSE_BUTTON_XBUTTON1):
		print("Mouse button held down")
	
	# mouse held down
	if GlobalInput.is_global_mouse_button_just_released(MOUSE_BUTTON_XBUTTON1):
		print("Mouse button just released")
	
	# input action just pressed
	if GlobalInput.is_global_input_just_pressed("ui_accept"):
		print("ui_accept action just pressed")
	
	# input action held down
	if GlobalInput.is_global_input_pressed("ui_up"):
		# checks all keys bound to ui_up (w, up arrow, etc.)
		print("ui_up action is pressing")
	
	#var pos = GlobalInput.get_global_mouse_position()
	#print("Global mouse position: ", pos)
