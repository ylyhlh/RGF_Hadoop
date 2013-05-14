import ubigraph
import time
color_table = [
"#800000",
"#8B0000",
"#A52A2A",
"#B22222",
"#DC143C",
"#FF0000",
"#FF6347",
"#FF7F50",
"#CD5C5C",
"#F08080",
"#E9967A",
"#FA8072",
"#FFA07A",
"#FF4500",
"#FF8C00",
"#FFA500",
"#FFD700",
"#B8860B",
"#DAA520",
"#EEE8AA",
"#BDB76B",
"#F0E68C",
"#808000",
"#FFFF00",
"#9ACD32",
"#556B2F",
"#6B8E23",
"#7CFC00",
"#7FFF00",
"#ADFF2F",
"#006400",
"#008000",
"#228B22",
"#00FF00",
"#32CD32",
"#90EE90",
"#98FB98",
"#8FBC8F",
"#00FA9A",
"#00FF7F",
"#2E8B57",
"#66CDAA",
"#3CB371",
"#20B2AA",
"#2F4F4F",
"#008080",
"#008B8B",
"#00FFFF",
"#00FFFF",
"#E0FFFF",
"#00CED1",
"#40E0D0",
"#48D1CC",
"#AFEEEE",
"#7FFFD4",
"#B0E0E6",
"#5F9EA0",
"#4682B4",
"#6495ED",
"#00BFFF",
"#1E90FF",
"#ADD8E6",
"#87CEEB",
"#87CEFA",
"#191970",
"#000080",
"#00008B",
"#0000CD",
"#0000FF",
"#4169E1",
"#8A2BE2",
"#4B0082",
"#483D8B",
"#6A5ACD",
"#7B68EE",
"#9370DB",
"#8B008B",
"#9400D3",
"#9932CC",
"#BA55D3",
"#800080",
"#D8BFD8",
"#DDA0DD",
"#EE82EE",
"#FF00FF",
"#DA70D6",
"#C71585",
"#DB7093",
"#FF1493",
"#FF69B4",
"#FFB6C1",
"#FFC0CB",
"#FAEBD7",
"#F5F5DC",
"#FFE4C4",
"#FFEBCD",
"#F5DEB3",
"#FFF8DC",
"#FFFACD",
"#FAFAD2",
"#FFFFE0",
"#8B4513",
"#A0522D",
"#D2691E",
"#CD853F",
"#F4A460",
"#DEB887",
"#D2B48C",
"#BC8F8F",
"#FFE4B5",
"#FFDEAD",
"#FFDAB9",
"#FFE4E1",
"#FFF0F5",
"#FAF0E6",
"#FDF5E6",
"#FFEFD5",
"#FFF5EE",
"#F5FFFA"]

ROOT = -1
class Vectex:
  def __init__(self, name):
    self.name = name
    if '-' in name:
      tree, node = name.split('-')
      self.tree = int(tree)
      self.node = int(node)
      self.id = self.tree * 1000000 + self.node
      self.color_code = color_table[self.tree % len(color_table)]
    elif name == "root":
      self.id = ROOT
    else:
      print "opt as a vertex"
      self.id = None

  def draw_vertex(self, graph):
    graph.newVertex(id = self.id, label=self.name, size=2, color=self.color_code)

class DrawEvent:
  def __init__(self, t, parent, child):
    """If child is None, it is an OPT event."""
    self.parent = parent
    self.child = child
    self.t = t

  def draw(self, graph):
    if self.child is not None: # root or nodes
      self.child.draw_vertex(graph)
      # if ret != 0:
      #   print "draw event error", ret
      graph.newEdge(self.parent, self.child, arrow=True)
    else:
      print "Drawing opt event...", self.parent.name


def read_event(line):
  words = line.split()
  t = float(words[1])
  if words[2] != "opt":
    e = DrawEvent(t, Vectex(words[2]), Vectex(words[3]))
  else:
    e = DrawEvent(t, Vectex(words[2]), None)
  return e

def draw(events_list):
  # Create an object to represent our server.
  G = ubigraph.Ubigraph()
  G.clear()
  # Start
  G.newVertex(id = ROOT, label="root", size=5)
  start_time = time.time()
  while events_list: # There are some event to draw
    # check time
    elapsed_time = time.time() - start_time
    # print elapsed_time
    while events_list and elapsed_time > events_list[0].t:
      e = events_list.pop(0)
      e.draw(G)
    time.sleep(0.01)

# read event list file
def read_events_list(filename):
  events = []
  with open(filename, 'r') as fin:
    for line in fin:
      if line and line.startswith("@VIS"):
        events.append(read_event(line))

  # for i in xrange(10):
  #   print events[i].t
  return events

def main():
  events = read_events_list("vis12.txt")
  draw(events)

if __name__ == "__main__":
  main()