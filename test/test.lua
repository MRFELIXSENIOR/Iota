Iota.Input.OnKeyDown.Connect(function(key)
	if (Iota.Enum.KeyCode.A == key) then
		print("A key is pressed")
	end
end)

local boolthing = true

Iota.Input.OnKeyDown.Connect(function(key)
	if (Iota.Enum.KeyCode.G == key) then
		boolthing = false
		print("stopped")
	end
end)

print(Iota.Input.IsKeyDown(Iota.Enum.KeyCode.Q));