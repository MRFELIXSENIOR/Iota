print('from lua!!!!!')

local texture = Texture.new()
texture:LoadTexture("funny.png")

Iota.GetRenderer():RenderTexture(texture)
