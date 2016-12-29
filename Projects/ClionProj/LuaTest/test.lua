--trace("aaa")
--say_res = say("aaa")
--say(say_res)
--
--function main(num)
--    trace("main function called")
--    trace(string.format("Num => %f",num))
--end

--world = createWorld()
--
--gameObject = world.createGameObject()
--gameObject.update = function (timeInSecs)
--    gameObject.transform.position = vec3(1,0,1);
--end
--gameObject.render = function ()
--
--end

function init()
    light = Light:create()
    light.color =
    world:addNode(light)
end

function update(timeInSecs)

end