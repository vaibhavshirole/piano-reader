// Constants
const STAFF_WIDTH = 800;
const NOTE_SPACING = 100;
const NOTES = [
  // Bass clef notes (C2 to B3)
  'C2', 'D2', 'E2', 'F2', 'G2', 'A2', 'B2',
  'C3', 'D3', 'E3', 'F3', 'G3', 'A3', 'B3',
  // Treble clef notes (C4 to C6)
  'C4', 'D4', 'E4', 'F4', 'G4', 'A4', 'B4',
  'C5', 'D5', 'E5', 'F5', 'G5', 'A5', 'B5', 'C6'
];

// Position mapping for notes (distance from bottom line in steps)
const NOTE_POSITIONS = {
  // Bass clef (positions relative to bass staff)
  'C2': 11, 'D2': 10, 'E2': 9, 'F2': 8, 'G2': 7, 'A2': 6, 'B2': 5,
  'C3': 4, 'D3': 3, 'E3': 2, 'F3': 1, 'G3': 0, 'A3': -1, 'B3': -2,
  // Treble clef (positions relative to treble staff)
  'C4': 7, 'D4': 6, 'E4': 5, 'F4': 4, 'G4': 3, 'A4': 2, 'B4': 1,
  'C5': 0, 'D5': -1, 'E5': -2, 'F5': -3, 'G5': -4, 'A5': -5, 'B5': -6, 'C6': -7
};

const SCROLL_SPEEDS = {
  'Very Slow': 0.5,
  'Slow': 1,
  'Medium': 1.5,
  'Fast': 2,
  'Very Fast': 3
};

// Main Component
const SightreadingTrainer = () => {
  const [port, setPort] = React.useState(null);
  const [availablePorts, setAvailablePorts] = React.useState([]);
  const [baudRate, setBaudRate] = React.useState('9600');
  const [isConnected, setIsConnected] = React.useState(false);
  const [notes, setNotes] = React.useState([]);
  const [scrollPosition, setScrollPosition] = React.useState(0);
  const [isScrolling, setIsScrolling] = React.useState(false);
  const [currentNoteIndex, setCurrentNoteIndex] = React.useState(0);
  const [error, setError] = React.useState('');
  const [scrollMode, setScrollMode] = React.useState('auto'); // 'auto' or 'input'
  const [scrollSpeed, setScrollSpeed] = React.useState('Medium');
  
  const animationFrameRef = React.useRef();
  const reader = React.useRef(null);

  React.useEffect(() => {
    const listPorts = async () => {
      try {
        const ports = await navigator.serial.getPorts();
        setAvailablePorts(ports);
      } catch (err) {
        setError('Serial API not supported in this browser');
      }
    };
    
    listPorts();
    return () => {
      if (port) {
        port.close();
      }
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }
    };
  }, []);

  const generateNotes = () => {
    const newNotes = Array(20).fill(null).map(() => {
      const randomIndex = Math.floor(Math.random() * NOTES.length);
      return NOTES[randomIndex];
    });
    setNotes(newNotes);
    setCurrentNoteIndex(0);
    setScrollPosition(0);
  };

  const connectPort = async () => {
    try {
      const selectedPort = await navigator.serial.requestPort();
      await selectedPort.open({ baudRate: parseInt(baudRate) });
      setPort(selectedPort);
      setIsConnected(true);
      startReading(selectedPort);
    } catch (err) {
      setError(`Failed to connect: ${err.message}`);
    }
  };

  const disconnectPort = async () => {
    if (port) {
      await port.close();
      setPort(null);
      setIsConnected(false);
    }
  };

  const startReading = async (selectedPort) => {
    reader.current = selectedPort.readable.getReader();
    const decoder = new TextDecoder();
    
    try {
      while (true) {
        const { value, done } = await reader.current.read();
        if (done) break;
        
        const note = decoder.decode(value).trim();
        if (note === notes[currentNoteIndex]) {
          setCurrentNoteIndex(prev => prev + 1);
          if (scrollMode === 'input') {
            setScrollPosition(prev => prev + NOTE_SPACING);
          }
          if (currentNoteIndex >= notes.length - 5) {
            setNotes(prev => [...prev, ...generateMoreNotes(5)]);
          }
        }
      }
    } catch (err) {
      setError(`Error reading from port: ${err.message}`);
    }
  };

  const generateMoreNotes = (count) => {
    return Array(count).fill(null).map(() => {
      const randomIndex = Math.floor(Math.random() * NOTES.length);
      return NOTES[randomIndex];
    });
  };

  const startScrolling = () => {
    if (scrollMode === 'auto') {
      setIsScrolling(true);
      animate();
    }
  };

  const stopScrolling = () => {
    setIsScrolling(false);
    if (animationFrameRef.current) {
      cancelAnimationFrame(animationFrameRef.current);
    }
  };

  const animate = () => {
    if (!isScrolling) return;
    
    setScrollPosition(prev => {
      const newPosition = prev + SCROLL_SPEEDS[scrollSpeed];
      return newPosition;
    });
    
    if (isScrolling) {
      animationFrameRef.current = requestAnimationFrame(animate);
    }
  };


  React.useEffect(() => {
    if (isScrolling) {
      animationFrameRef.current = requestAnimationFrame(animate);
    }
    return () => {
      if (animationFrameRef.current) {
        cancelAnimationFrame(animationFrameRef.current);
      }
    };
  }, [isScrolling]);

  const isNoteInBassClef = (note) => {
    return note.match(/[A-G][2-3]/);
  };

  const renderStaff = () => {
    return (
      <svg className="w-full h-96" viewBox={`${scrollPosition} 0 ${STAFF_WIDTH} 200`}>
        {/* Bass Clef Staff */}
        <g transform="translate(0, 120)">
          {[0, 10, 20, 30, 40].map((y, i) => (
            <line
              key={`bass-${i}`}
              x1="0"
              y1={y}
              x2={STAFF_WIDTH + scrollPosition}
              y2={y}
              stroke="black"
              strokeWidth="1"
            />
          ))}
          {/* Bass clef symbol */}
          <text x={scrollPosition} y="30" fontSize="40">𝄢</text>
        </g>

        {/* Treble Clef Staff */}
        <g transform="translate(0, 40)">
          {[0, 10, 20, 30, 40].map((y, i) => (
            <line
              key={`treble-${i}`}
              x1="0"
              y1={y}
              x2={STAFF_WIDTH + scrollPosition}
              y2={y}
              stroke="black"
              strokeWidth="1"
            />
          ))}
          {/* Treble clef symbol */}
          <text x={scrollPosition} y="35" fontSize="40">𝄞</text>
        </g>

        {/* Notes */}
        {notes.map((note, i) => {
          const x = (i * NOTE_SPACING) + 100;
          const isBassPart = isNoteInBassClef(note);
          const baseY = isBassPart ? 120 : 40;
          const y = baseY + (NOTE_POSITIONS[note] * 5 + 20);
          
          return (
            <g key={i} transform={`translate(${x}, ${y})`}>
              <ellipse
                cx="0"
                cy="0"
                rx="6"
                ry="4"
                fill={i < currentNoteIndex ? '#4CAF50' : 'black'}
                transform="rotate(-20)"
              />
              {/* Add stem */}
              <line
                x1="6"
                y1="0"
                x2="6"
                y2={isBassPart ? "20" : "-20"}
                stroke={i < currentNoteIndex ? '#4CAF50' : 'black'}
                strokeWidth="1"
              />
              {/* Add ledger lines */}
              {Array.from({ length: Math.abs(NOTE_POSITIONS[note]) }).map((_, j) => {
                if (NOTE_POSITIONS[note] < -5 || NOTE_POSITIONS[note] > 5) {
                  return (
                    <line
                      key={`ledger-${i}-${j}`}
                      x1="-8"
                      y1={j * 10}
                      x2="8"
                      y2={j * 10}
                      stroke="black"
                      strokeWidth="1"
                    />
                  );
                }
                return null;
              })}
            </g>
          );
        })}
      </svg>
    );
  };

  return (
    <div className="p-4 max-w-4xl mx-auto">
      <h1 className="text-2xl font-bold mb-4">Sightreading Trainer</h1>
      
      <div className="mb-4 flex gap-4">
        <select 
          className="border p-2 rounded"
          value={baudRate}
          onChange={(e) => setBaudRate(e.target.value)}
        >
          <option value="9600">9600</option>
          <option value="115200">115200</option>
        </select>
        
        {isConnected ? (
          <button
            className="bg-red-500 text-white px-4 py-2 rounded"
            onClick={disconnectPort}
          >
            Disconnect
          </button>
        ) : (
          <button
            className="bg-blue-500 text-white px-4 py-2 rounded"
            onClick={connectPort}
          >
            Connect
          </button>
        )}
      </div>

      {error && (
        <div className="bg-red-100 border border-red-400 text-red-700 px-4 py-3 rounded mb-4">
          {error}
        </div>
      )}

      <div className="mb-4 flex gap-4">
        <select
          className="border p-2 rounded"
          value={scrollMode}
          onChange={(e) => setScrollMode(e.target.value)}
        >
          <option value="auto">Auto Scroll</option>
          <option value="input">Scroll on Input</option>
        </select>

        {scrollMode === 'auto' && (
          <select
            className="border p-2 rounded"
            value={scrollSpeed}
            onChange={(e) => setScrollSpeed(e.target.value)}
          >
            {Object.keys(SCROLL_SPEEDS).map(speed => (
              <option key={speed} value={speed}>{speed}</option>
            ))}
          </select>
        )}
      </div>

      <div className="border p-4 rounded mb-4 bg-white">
        {renderStaff()}
      </div>

      <div className="flex gap-4">
        <button
          className="bg-green-500 text-white px-4 py-2 rounded"
          onClick={generateNotes}
        >
          Regenerate
        </button>
        {scrollMode === 'auto' && (
          <>
            <button
              className="bg-blue-500 text-white px-4 py-2 rounded"
              onClick={startScrolling}
              disabled={!notes.length || isScrolling}
            >
              Start
            </button>
            <button
              className="bg-red-500 text-white px-4 py-2 rounded"
              onClick={stopScrolling}
              disabled={!isScrolling}
            >
              Stop
            </button>
          </>
        )}
      </div>
    </div>
  );
};

// Render the app
ReactDOM.render(
  <SightreadingTrainer />,
  document.getElementById('root')
);