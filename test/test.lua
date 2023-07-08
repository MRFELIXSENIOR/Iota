print('from lua!!!!!')

Iota.Input.OnKeyDown.Connect(function(key)
	print("You Pressed: ", Iota.Utilities.ConvertKeyCode(key))
end)

Iota.Input.OnKeyRelease.Connect(function(key)
	print("You Released: ", Iota.Utilities.ConvertKeyCode(key))
end)
