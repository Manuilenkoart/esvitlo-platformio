import { useEffect, useState } from "preact/hooks";
import "./app.css";

type Ping = {
  id: number;
  voltage: "ON" | "OFF";
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
      setPing(() => ({ ...json, time }));
    });

    eventSource.addEventListener("error", (e) => {
      console.log(e);
    });
  }, []);

  return (
    <div class="min-h-screen bg-gradient-to-br from-slate-900 to-slate-700 flex items-center justify-center p-4">
      <div class="bg-white/10 backdrop-blur-md rounded-2xl p-8 shadow-2xl border border-white/20 max-w-md w-full">
        <div class="text-center mb-8">
          <h1 class="text-2xl font-bold text-white mb-2">Power Monitor</h1>
          <div
            class={`inline-flex items-center px-4 py-2 rounded-full font-semibold ${
              ping?.voltage === "ON"
                ? "bg-green-500/20 text-green-400 border border-green-500/30"
                : "bg-red-500/20 text-red-400 border border-red-500/30"
            }`}
          >
            <div
              class={`w-2 h-2 rounded-full mr-2 ${
                ping?.voltage === "ON" ? "bg-green-400" : "bg-red-400"
              }`}
            ></div>
            {ping?.voltage || "Unknown"}
          </div>
        </div>

        <div class="space-y-6">
          <div class="bg-white/5 rounded-xl p-4 border border-white/10">
            <div class="flex items-center justify-between">
              <span class="text-slate-300 text-sm">Last Updated</span>
              <svg
                class="w-4 h-4 text-slate-400"
                fill="none"
                stroke="currentColor"
                viewBox="0 0 24 24"
              >
                <path
                  stroke-linecap="round"
                  stroke-linejoin="round"
                  stroke-width="2"
                  d="M12 8v4l3 3m6-3a9 9 0 11-18 0 9 9 0 0118 0z"
                ></path>
              </svg>
            </div>
            <div class="text-white font-medium mt-1">
              {ping?.time ? new Date(ping.time).toLocaleString() : "Never"}
            </div>
          </div>

          <div class="bg-white/5 rounded-xl p-4 border border-white/10">
            <div class="flex items-center justify-between">
              <span class="text-slate-300 text-sm">WiFi Signal</span>
              <svg
                class="w-4 h-4 text-slate-400"
                fill="none"
                stroke="currentColor"
                viewBox="0 0 24 24"
              >
                <path
                  stroke-linecap="round"
                  stroke-linejoin="round"
                  stroke-width="2"
                  d="M8.111 16.404a5.5 5.5 0 017.778 0M12 20h.01m-7.08-7.071c3.904-3.905 10.236-3.905 14.141 0M1.394 9.393c5.857-5.857 15.355-5.857 21.213 0"
                ></path>
              </svg>
            </div>
            <div class="flex items-center mt-1">
              <span class="text-white font-medium mr-2">
                {ping?.rssi ? `${ping.rssi} dBm` : "Unknown"}
              </span>
              <div class="flex space-x-1">
                {[1, 2, 3, 4].map((bar) => (
                  <div
                    key={bar}
                    class={`w-1 h-3 rounded-full ${
                      ping?.rssi && parseInt(ping.rssi) > -50 - bar * 20
                        ? "bg-green-400"
                        : "bg-slate-600"
                    }`}
                  ></div>
                ))}
              </div>
            </div>
          </div>

          <div class="bg-white/5 rounded-xl p-4 border border-white/10">
            <div class="flex items-center justify-between">
              <span class="text-slate-300 text-sm">Device ID</span>
              <svg
                class="w-4 h-4 text-slate-400"
                fill="none"
                stroke="currentColor"
                viewBox="0 0 24 24"
              >
                <path
                  stroke-linecap="round"
                  stroke-linejoin="round"
                  stroke-width="2"
                  d="M9 12h6m-6 4h6m2 5H7a2 2 0 01-2-2V5a2 2 0 012-2h5.586a1 1 0 01.707.293l5.414 5.414a1 1 0 01.293.707V19a2 2 0 01-2 2z"
                ></path>
              </svg>
            </div>
            <div class="text-white font-medium mt-1">
              {ping?.id || "Unknown"}
            </div>
          </div>
        </div>
      </div>
    </div>
  );
}
