export default function FormField({ label, className = "", ...props }) {
  return (
    <label className="block space-y-2">
      <span className="text-sm font-medium text-slate-300">{label}</span>
      <input
        {...props}
        className={`w-full rounded-2xl border border-white/10 bg-white/5 px-4 py-3 text-sm text-white outline-none transition placeholder:text-slate-500 focus:border-aqua/60 focus:bg-white/10 focus:shadow-[0_0_0_4px_rgba(77,246,210,0.1)] ${className}`}
      />
    </label>
  );
}
