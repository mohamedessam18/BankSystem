import { motion } from "framer-motion";

export default function GlassCard({ children, className = "" }) {
  return (
    <motion.div
      initial={{ opacity: 0, y: 18 }}
      animate={{ opacity: 1, y: 0 }}
      transition={{ duration: 0.45 }}
      className={`rounded-3xl border border-white/10 bg-white/[0.06] p-6 shadow-panel backdrop-blur-2xl ${className}`}
    >
      {children}
    </motion.div>
  );
}
