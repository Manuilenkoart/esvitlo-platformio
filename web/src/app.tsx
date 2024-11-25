import { useEffect, useState } from "preact/hooks";
import preactLogo from "./assets/preact.svg";
import viteLogo from "/vite.svg";
import "./app.css";

type Ping = {
  name: "open" | "ping" | "error";
  id: string;
  voltage: "ON" | "OFF";
  timestamp: string;
};
export function App() {
  const [ping, setPing] = useState<Ping | null>(null);

  useEffect(() => {
    const sse = new EventSource("/events");

    sse.addEventListener("open", (e) => {
      console.log(e);
    });

    sse.addEventListener("ping", (e) => {
      const json = JSON.parse(e.data);
      setPing(json);
    });
    sse.addEventListener("error", (e) => {
      console.error(e);
    });
  }, []);

  return (
    <>
      <div>
        <a href="https://vite.dev" target="_blank">
          <img src={viteLogo} class="logo" alt="Vite logo" />
        </a>
        <a href="https://preactjs.com" target="_blank">
          <img src={preactLogo} class="logo preact" alt="Preact logo" />
        </a>
      </div>
      <h1>ping: {ping?.id}</h1>
      <h1>voltage: {ping?.voltage}</h1>
    </>
  );
}
