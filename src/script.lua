
screen_width =  640
screen_height = 480

function draw(dt)
    local center_x = screen_width / 2
    local center_y = screen_height / 2

    for i = 1, 300 do
        local x = math.floor(i * math.sin(i * dt))
        local y = math.floor(i * math.cos(i * dt))
        drawpoint(center_x - x, center_y - y)
    end
end