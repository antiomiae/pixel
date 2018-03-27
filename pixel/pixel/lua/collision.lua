--

local function collide_rect(r, d, tile_f)
    -- tile_f = f(x, y) returning tile

    local function tile_row(row_num, start, stop)
        out = {}

        for x = start, stop do
            table.insert(out, tile_f(x, row_num))
        end

        return out
    end

    local function tile_col(col_num, start, stop)
        out = {}

        for y = start, stop do
            table.insert(out, tile_f(col_num, y))
        end

        return out
    end

    -- assume rect is not colliding with any solid tiles
    -- calculate distance between leading edges and tile boundaries
    -- move r along trajectory until it hits a tile edge
    -- subtract distance traveled from displacement vector
    -- 
end
