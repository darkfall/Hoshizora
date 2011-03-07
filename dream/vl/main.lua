--- Library

function Class(members)
  members = members or {}
  local mt = {
    __metatable = members;
    __index     = members;
  }
  local function new(_, init)
    return setmetatable(init or {}, mt)
  end
  local function copy(obj, ...)
    local newobj = obj:new(unpack(arg))
    for n,v in pairs(obj) do newobj[n] = v end
    return newobj
  end
  members.new  = members.new  or new
  members.copy = members.copy or copy
  return mt
end

Vector = {}

local Vector_mt = Class(Vector)

function Vector:new(x,y)
  return setmetatable( {x=x, y=y}, Vector_mt)
end

function Vector:mag()
  return math.sqrt(self:dot(self))
end

function Vector:dot(v)
  return self.x * v.x + self.y * v.y
end

function Vector:normalize()
	self.x,self.y = self.x/self:mag(),self.y/self:mag()
end

function Vector:angle(x,y)
	if y then x=Vector:new(x,y) end
	return math.acos(self:dot(x))
end
--- Program

sintable,costable={},{}
division = 100

function love.load()
	for 
end