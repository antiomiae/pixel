limit = ARGV.shift&.to_i
raise unless limit

min, max = ARGV.shift.split('x').map(&:to_i)
raise unless min and max

sizes = limit.times.map { r = 2**rand(min..max); [(r * rand(1.0 .. 2.0)).to_i - 2, (r * rand(1.0 .. 2.0)).to_i - 2] }

def image(name, w, h)
    cmd = "convert -colorspace RGB -type TrueColorAlpha -size #{w}x#{h} xc:'hsl(#{rand(360)},#{rand(40..60)}%,#{rand(50..90)}%)' png32:#{name}"
    puts cmd
    %x(#{cmd})
end

prefix = ARGV.shift || 'square_'

sizes.each_with_index do |size, i|
  image("#{prefix}#{i}.png", *size)
end
