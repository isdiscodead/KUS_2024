import React, { useRef, useEffect, useState } from 'react';
import axios from 'axios';

const WebcamStream = () => {
  const videoRef = useRef(null);
  const [result, setResult] = useState(null);

  const captureFrame = async () => {
    const canvas = document.createElement("canvas");
    const context = canvas.getContext("2d");
    canvas.width = videoRef.current.videoWidth;
    canvas.height = videoRef.current.videoHeight;
    context.drawImage(videoRef.current, 0, 0, canvas.width, canvas.height);
    const frame = canvas.toDataURL("image/jpeg");

    // 서버로 프레임 전송
    axios.post('http://localhost:8000/api/process_frame/', 
      {
      frame: JSON.stringify({ frame })
      }, 
      {
        headers: {
          'Content-Type': 'application/json',
        },
      }).then((res) => {
          console.log(res.data);
          setResult(res);
      }).catch((err) => {
          console.error(err);
      });
    };

  useEffect(() => {
    const interval = setInterval(captureFrame, 100); // 10FPS
    return () => clearInterval(interval);
  }, []);

  return (
    <div>
      <video ref={videoRef} autoPlay muted playsInline />
      <div>{result && JSON.stringify(result)}</div>
    </div>
  );
};

export default WebcamStream;
