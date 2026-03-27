export function Spinner() {
  return <div className="h-5 w-5 animate-spin rounded-full border-2 border-slate-400/20 border-t-current" />;
}

export function Skeleton({ className = "" }) {
  return <div className={`animate-pulse rounded-2xl bg-white/[0.08] ${className}`} />;
}
