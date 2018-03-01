limit = ARGV.shift&.to_i
raise unless limit

min, max = ARGV.shift.split('x').map(&:to_i)
raise unless min and max

sizes = limit.times.map { r = 2**rand(min..max); [(r * rand(1.0 .. 2.0)).to_i - 2, (r * rand(1.0 .. 2.0)).to_i - 2] }

def image(name, w, h)
  `convert -colorspace RGB -type TrueColorAlpha -size #{w}x#{h} xc:white -bordercolor 'rgb(0,0,0)' -border 1 png32:#{name}`
end

prefix = ARGV.shift || 'square_'

sizes.each_with_index do |size, i|
  image("#{prefix}#{i}.png", *size)
end
