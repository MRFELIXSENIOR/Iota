print('from lua!!!!!')

local obj = Object.new()
obj:SetColorRGB(255, 255, 255)


Iota.Input.OnKeyDown:Connect(function(key)
	if key == Enum.KeyCode.A then
		obj.x.Value = obj.x.Value - 2
	elseif key == Enum.KeyCode.D then
		obj.x.Value = obj.x.Value + 2
	elseif key == Enum.KeyCode.W then
		obj.y.Value = obj.y.Value - 2
	elseif key == Enum.KeyCode.S then
		obj.y.Value = obj.y.Value + 2
	end
end)
