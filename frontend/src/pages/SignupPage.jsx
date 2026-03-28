import { useState } from "react";
import { motion } from "framer-motion";
import { Link, useNavigate } from "react-router-dom";
import FormField from "../components/FormField";
import { Spinner } from "../components/Loader";
import { useAuth } from "../context/AuthContext";
import { useToast } from "../context/ToastContext";

export default function SignupPage() {
  const [form, setForm] = useState({ role: "client", name: "", password: "", initialBalance: "1500" });
  const { signup, loading } = useAuth();
  const { pushToast } = useToast();
  const navigate = useNavigate();

  const submit = async (event) => {
    event.preventDefault();
    try {
      const payload = {
        role: form.role,
        name: form.name,
        password: form.password,
        initialBalance: Number(form.initialBalance)
      };

      const session = await signup(payload);
      pushToast({ title: `Welcome ${session.user.name}. Your ID is ${session.user.id}.` });
      navigate(`/dashboard/${session.role}`);
    } catch (error) {
      pushToast({ title: error.message, type: "error" });
    }
  };

  return (
    <div className="min-h-screen bg-ink bg-mesh px-4 py-10 text-white">
      <motion.div initial={{ opacity: 0, y: 24 }} animate={{ opacity: 1, y: 0 }} className="mx-auto max-w-md rounded-[2rem] border border-white/10 bg-white/[0.06] p-8 shadow-panel backdrop-blur-2xl">
        <p className="text-sm uppercase tracking-[0.4em] text-aqua">Client onboarding</p>
        <h1 className="mt-4 font-display text-4xl">Sign Up</h1>
        <p className="mt-3 text-sm leading-7 text-slate-300">Create a secure client account. Employee, admin, and manager access are provisioned internally and do not appear here.</p>
        <form className="mt-8 space-y-5" onSubmit={submit}>
          <FormField label="Name" required value={form.name} onChange={(event) => setForm((current) => ({ ...current, name: event.target.value }))} placeholder="Full name" />
          <FormField label="Password" type="password" required value={form.password} onChange={(event) => setForm((current) => ({ ...current, password: event.target.value }))} placeholder="8-20 characters, no spaces" />
          <FormField
            label="Initial Balance"
            type="number"
            min="1500"
            required
            value={form.initialBalance}
            onChange={(event) => setForm((current) => ({ ...current, initialBalance: event.target.value }))}
          />
          <button disabled={loading} className="flex w-full items-center justify-center gap-3 rounded-2xl bg-aqua px-4 py-3 font-semibold text-ink transition hover:shadow-glow disabled:cursor-not-allowed disabled:opacity-70">
            {loading ? <Spinner /> : "Create client account"}
          </button>
        </form>
        <p className="mt-6 text-sm text-slate-400">
          Already have an account?{" "}
          <Link className="text-aqua" to="/login">
            Login
          </Link>
        </p>
      </motion.div>
    </div>
  );
}
