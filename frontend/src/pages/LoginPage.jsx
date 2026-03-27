import { useState } from "react";
import { motion } from "framer-motion";
import { Link, useNavigate } from "react-router-dom";
import FormField from "../components/FormField";
import { Spinner } from "../components/Loader";
import { useAuth } from "../context/AuthContext";
import { useToast } from "../context/ToastContext";

export default function LoginPage() {
  const [form, setForm] = useState({ id: "", password: "" });
  const { login, loading } = useAuth();
  const { pushToast } = useToast();
  const navigate = useNavigate();

  const submit = async (event) => {
    event.preventDefault();
    try {
      const session = await login({ id: Number(form.id), password: form.password });
      pushToast({ title: "Authenticated successfully." });
      navigate(`/dashboard/${session.role}`);
    } catch (error) {
      pushToast({ title: error.message, type: "error" });
    }
  };

  return (
    <div className="min-h-screen bg-ink bg-mesh px-4 py-10 text-white">
      <motion.div initial={{ opacity: 0, y: 24 }} animate={{ opacity: 1, y: 0 }} className="mx-auto max-w-md rounded-[2rem] border border-white/10 bg-white/[0.06] p-8 shadow-panel backdrop-blur-2xl">
        <p className="text-sm uppercase tracking-[0.4em] text-aqua">Secure entry</p>
        <h1 className="mt-4 font-display text-4xl">Login</h1>
        <p className="mt-3 text-sm leading-7 text-slate-300">Use your bank ID and password to access your workspace.</p>
        <form className="mt-8 space-y-5" onSubmit={submit}>
          <FormField label="ID" type="number" min="1" required value={form.id} onChange={(event) => setForm((current) => ({ ...current, id: event.target.value }))} placeholder="Enter your user ID" />
          <FormField label="Password" type="password" required value={form.password} onChange={(event) => setForm((current) => ({ ...current, password: event.target.value }))} placeholder="Enter your password" />
          <button disabled={loading} className="flex w-full items-center justify-center gap-3 rounded-2xl bg-aqua px-4 py-3 font-semibold text-ink transition hover:shadow-glow disabled:cursor-not-allowed disabled:opacity-70">
            {loading ? <Spinner /> : "Access account"}
          </button>
        </form>
        <p className="mt-6 text-sm text-slate-400">
          Need a client account?{" "}
          <Link className="text-aqua" to="/signup">
            Sign up here
          </Link>
        </p>
      </motion.div>
    </div>
  );
}
