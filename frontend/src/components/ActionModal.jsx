import { AnimatePresence, motion } from "framer-motion";

export default function ActionModal({ open, title, children, onClose }) {
  return (
    <AnimatePresence>
      {open && (
        <motion.div
          initial={{ opacity: 0 }}
          animate={{ opacity: 1 }}
          exit={{ opacity: 0 }}
          className="fixed inset-0 z-40 flex items-center justify-center bg-slate-950/60 px-4 backdrop-blur-md"
        >
          <motion.div
            initial={{ opacity: 0, y: 24, scale: 0.98 }}
            animate={{ opacity: 1, y: 0, scale: 1 }}
            exit={{ opacity: 0, y: 18, scale: 0.98 }}
            className="w-full max-w-lg rounded-[2rem] border border-white/10 bg-shell/95 p-6 shadow-panel"
          >
            <div className="mb-6 flex items-center justify-between">
              <h3 className="font-display text-2xl text-white">{title}</h3>
              <button onClick={onClose} className="text-sm text-slate-400 transition hover:text-white">
                Close
              </button>
            </div>
            {children}
          </motion.div>
        </motion.div>
      )}
    </AnimatePresence>
  );
}
