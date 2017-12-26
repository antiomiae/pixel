require 'pp'
require 'pry'

f = File.readlines('lines').map(&:strip).map { |s| s.split(' ') }.map { |d, n| [d, n.to_i(16)] }
f.select! do |s, _|
    s.match? /^[A-Z0-9_]+$/
end

f.reject! do |s, _|
    s.match? /(_NV$)|(^GL_VERSION_)/
end

mapped = f.group_by { |name, num| num }.transform_values { |a| a.map { |s, _| s }.sort }

def emplace_line(id, key, values)
  "#{id}[#{key}] = {\"#{values.join('", "')}\"};"
end


source = <<~CPP
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

void populateSymbolMap(unordered_map<int, vector<string> >& symbolMap) {
    #{mapped.sort.map { |i, a| emplace_line("symbolMap", i, a) }.join("\n    ") }
}

CPP

puts source
