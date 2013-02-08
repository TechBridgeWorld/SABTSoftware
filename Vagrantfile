Vagrant::Config.run do |config|
  
  config.vm.customize [
    "modifyvm", :id,
    "--memory", "1024",
    "--cpus", 2
  ]

  config.vm.box = "precise64"
  config.vm.box_url = "http://files.vagrantup.com/precise64.box"
  
  config.vm.network :hostonly, "33.33.33.100"
  config.vm.forward_port 80, 8080
  
  # remove the next line when running on a windows host system (Windows does not have NFS support)
  config.vm.share_folder("v-root", "/vagrant", ".", :nfs => true)
end
