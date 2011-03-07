--[[
local _class={}


function class(...)
	local c = {}
	table.insert(_class,c)
	-- if there is only one superclass
	--[[if table.getn(arg) == 1 then setmetatable(c, {__index = function(t,k)
		return arg[1][k]
	end}) end
	if table.getn(arg) > 1 then --]]
	setmetatable(c, {__index = function(t,k)
		for i = 1, table.getn(arg) do
			local v = arg[i][k]
			if v then return v end
		end
	end}) --end
	c.__index = c
	function c:new(...)
		local o = {}
		setmetatable(o,c)
		--if c.__init then c.__init(o,...) end
		return o
	end
	
	return c
end

hi = class()
function hi:p()
	print ('hi')
end

function hi:__init(a)
	print (a)
end

hey = class(hi)
function hi:yo()
	print ('hey')
end

nah = class(hi,hey)
function nah:p()
	print ('wtf')
end


function nah:__init(a,b)
	--print (a)
	hi.__init(self,a)
	print (b*2)
end

nah:new('hi',4)

]]--