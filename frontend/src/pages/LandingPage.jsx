import { motion } from "framer-motion";
import { ArrowRight, ShieldCheck, Sparkles, Wallet } from "lucide-react";
import { Link } from "react-router-dom";

const features = [
  { icon: ShieldCheck, title: "Protected access", text: "Role-aware experiences for clients, employees, and admins." },
  { icon: Wallet, title: "Live transactions", text: "Deposits, withdrawals, transfers, and account operations in one flow." },
  { icon: Sparkles, title: "Fintech-grade UI", text: "Dark glass panels, polished motion, and responsive dashboards." }
];

export default function LandingPage() {
  return (
    <div className="relative min-h-screen overflow-hidden bg-ink text-white">
      <div className="absolute inset-0 bg-mesh opacity-90" />
      <div className="absolute left-1/2 top-20 h-80 w-80 -translate-x-1/2 rounded-full bg-aqua/10 blur-3xl" />
      <div className="relative mx-auto flex min-h-screen max-w-7xl flex-col px-6 py-8">
        <header className="flex items-center justify-between">
          <h1 className="font-display text-2xl">Secure Banking System</h1>
          <div className="flex gap-3">
            <Link className="rounded-full border border-white/10 px-5 py-3 text-sm text-slate-200 transition hover:bg-white/10" to="/login">
              Login
            </Link>
            <Link className="rounded-full bg-white px-5 py-3 text-sm font-semibold text-ink transition hover:scale-[1.02]" to="/signup">
              Sign Up
            </Link>
          </div>
        </header>

        <div className="grid flex-1 items-center gap-12 py-16 lg:grid-cols-[1.2fr_0.8fr]">
          <motion.div initial={{ opacity: 0, y: 32 }} animate={{ opacity: 1, y: 0 }} transition={{ duration: 0.7 }}>
            <p className="mb-5 text-sm uppercase tracking-[0.5em] text-aqua">Modern fintech platform</p>
            <h2 className="max-w-3xl font-display text-5xl leading-tight md:text-7xl">
              Confident banking operations with a dashboard that feels premium.
            </h2>
            <p className="mt-6 max-w-2xl text-lg leading-8 text-slate-300">
              A full-stack banking experience powered by C++ services, TXT persistence, and a frontend designed to feel like a modern financial product.
            </p>
            <div className="mt-10 flex flex-wrap gap-4">
              <Link to="/login" className="inline-flex items-center gap-2 rounded-full bg-aqua px-6 py-3 text-sm font-semibold text-ink transition hover:shadow-glow">
                Access Dashboard
                <ArrowRight className="h-4 w-4" />
              </Link>
              <Link to="/signup" className="rounded-full border border-white/10 px-6 py-3 text-sm text-slate-200 transition hover:bg-white/10">
                Create Client Account
              </Link>
            </div>
          </motion.div>

          <motion.div
            initial={{ opacity: 0, scale: 0.96 }}
            animate={{ opacity: 1, scale: 1 }}
            transition={{ delay: 0.15, duration: 0.7 }}
            className="rounded-[2rem] border border-white/10 bg-white/[0.06] p-6 shadow-panel backdrop-blur-2xl"
          >
            <div className="grid gap-4">
              {features.map(({ icon: Icon, title, text }) => (
                <div key={title} className="rounded-3xl border border-white/10 bg-slate-950/40 p-5">
                  <Icon className="mb-4 h-6 w-6 text-aqua" />
                  <h3 className="font-display text-xl">{title}</h3>
                  <p className="mt-2 text-sm leading-7 text-slate-300">{text}</p>
                </div>
              ))}
            </div>
          </motion.div>
        </div>
      </div>
    </div>
  );
}
