# -*- coding: utf-8 -*-

module ReVIEW
  module CodeRegion
    # Markers occupy a whole comment line; ordinary code/string literals do not match.
    MARKER = /\A\s*(?:\/\/|\#|--|;|%|\/\*|<!--|\(\*|\{-|\*)\s*book:(begin|end)\s+([A-Za-z0-9_.-]+)\s*(?:\*\/|-->|\*\)|-\})?\s*\z/

    def self.extract(lines, name, filename)
      stack = []
      seen = {}
      result = []
      lines.each_with_index do |line, index|
        marker = MARKER.match(line.chomp)
        unless marker
          result << line if stack.include?(name)
          next
        end
        action, label = marker.captures
        location = "#{filename}:#{index + 1}"
        if action == 'begin'
          raise ArgumentError, "#{location}: duplicate region #{label}" if seen[label]
          seen[label] = true
          stack << label
        else
          unless stack.last == label
            raise ArgumentError, "#{location}: unmatched region end #{label}"
          end
          stack.pop
        end
      end
      raise ArgumentError, "#{filename}: unclosed region #{stack.last}" unless stack.empty?
      raise ArgumentError, "#{filename}: region #{name} not found" unless seen[name]
      result
    end
  end
end
