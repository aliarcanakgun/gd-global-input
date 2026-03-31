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
	
	# input action just pressed
	if GlobalInput.is_global_input_just_pressed("ui_accept"):
		print("ui_accept action just pressed")
	
	# input action held down
	if GlobalInput.is_global_input_pressed("ui_up"):
		# checks all keys bound to ui_up (w, up arrow, etc.)
		print("ui_up action is pressing")
