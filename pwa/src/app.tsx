import { useEffect, useState } from "preact/hooks";
import "./app.css";

type Ping = {
  id: number;
  voltage: string;
  rssi: string;
  time: Date;
};
export function App() {
  const [ping, setPing] = useState<Ping | null>(null);

  useEffect(() => {
    const eventSource = new EventSource("/events");

    eventSource.addEventListener("open", (e) => {
      console.log(e);
    });
    eventSource.addEventListener("ping", (e) => {
      const json = JSON.parse(e.data);
      const time = new Date().toString();
      setPing(() => ({...json, time}));
    });

    eventSource.addEventListener("error", (e) => {
      console.log(e);
    });
  }, []);


  return (
    <div class={`container ${!!ping && ping.voltage === "ON" ? "yellow" : "blue"}`}>
      <div>voltage: {ping?.voltage}</div>
      <div>last updated at: {ping?.time}</div>
      <div>id: {ping?.id}</div>
      <div>wifi rsi: {ping?.rssi}</div>
    </div>
  );
}
